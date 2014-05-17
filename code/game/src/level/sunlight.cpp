#include "level/sunlight.hpp"
#include "options.hpp"
#include <timer.hpp>

using namespace std;

// WARNING Some bug prevents the sunlight from being cleaned up properly. static_sunlight works around this issue
PT(DirectionalLight) static_sunlight;

Sunlight::Sunlight(World& world, const TimeManager& time_manager) : world(world), time_manager(time_manager), ground_level(0)
{
  NodePath sun_root = world.floors_node;

  solar_orbit.SetFromWorld(&world);
  sun_root.set_two_sided(false);
  InitializeSun();
  InitializeAmbientLight();
  SetLightColorFromTime();
  SetSunlightPositionFromTime();
  SetShadowCaster();

  // TODO implement SetLightOnModels and remove this
  sun_root.set_light(sunlight_nodepath, 6);
  sun_root.set_light(ambient_nodepath, 5);

  Connect(OptionsManager::Updated, *this, &Sunlight::SetShadowCaster);
}

Sunlight::~Sunlight()
{
  world.floors_node.set_light_off(sunlight_nodepath);
  world.floors_node.set_light_off(ambient_nodepath);
  world.floors_node.clear_light();
  ambient_nodepath.remove_node();
  sunlight_nodepath.remove_node();
}

void Sunlight::SetNearFar(float distance_near, float distance_far)
{
  sunlight_node->get_lens()->set_near_far(distance_near, distance_far);
}

void Sunlight::InitializeSun()
{
  NodePath sun_root = world.floors_node;

  if (static_sunlight.is_null())
    static_sunlight = new DirectionalLight("sunlight");
  sunlight_node     = static_sunlight;
  sunlight_nodepath = sun_root.attach_new_node(sunlight_node);

  NodePath sphere   = sunlight_nodepath.attach_new_node("sun-representation");

  static_sunlight->show_frustum();
  world.model_sphere.copy_to(sphere);
  sphere.set_scale(10);
  sunlight_nodepath.reparent_to(sphere);
  sunlight_nodepath.show();
}

void Sunlight::InitializeAmbientLight()
{
  NodePath sun_root = world.floors_node;

  ambient_node      = new AmbientLight("ambient_sunlight");
  ambient_nodepath  = sun_root.attach_new_node(ambient_node);
}

void Sunlight::SetShadowCaster()
{
  unsigned int shadow_caster_buffer = GetShadowBufferSize();

  sunlight_node->set_shadow_caster(true, shadow_caster_buffer, shadow_caster_buffer);
  sunlight_node->get_lens()->set_near_far(10.f, solar_orbit.GetRadius() * 4);
  sunlight_node->get_lens()->set_film_size(1024);
}

unsigned int Sunlight::GetShadowBufferSize(void)
{
  unsigned int shadow_buffer_size = 256;
  short        graphics_quality = OptionsManager::Get()["graphics-quality"];

  while (graphics_quality > 0)
  {
    shadow_buffer_size *= 2;
    graphics_quality--;
  }
  return (8192);
}

unsigned int Sunlight::GetFilmSize()
{
  unsigned int film_size        = 128;
  unsigned int graphics_quality = OptionsManager::Get()["graphics-quality"];

  film_size += 128 * (graphics_quality + 1);
  return (film_size);
}

void Sunlight::SetLightsOnModels()
{
}

void Sunlight::Run()
{
  if (!sunlight_nodepath.is_empty() && time_since_last_run.GetElapsedTime() > 0.01f)
  {
    SetLightColorFromTime();
    SetSunlightPositionFromTime();
    time_since_last_run.Restart();
  }
  ScheduledTask::Run();
}

void Sunlight::SetLightColorFromTime(void)
{
  /*LVecBase4f color_steps[6]            = {
    LVecBase4f(0.2, 0.2, 0.5, 1), // 00h00
    LVecBase4f(0.2, 0.2, 0.3, 1), // 04h00
    LVecBase4f(0.7, 0.7, 0.5, 1), // 08h00
    LVecBase4f(1.0, 1.0, 1.0, 1), // 12h00
    LVecBase4f(1.0, 0.8, 0.8, 1), // 16h00
    LVecBase4f(0.4, 0.4, 0.6, 1)  // 20h00
  };*/
  LVecBase4f color_steps[5]            = {
    LVecBase4f(0.2, 0.2, 0.5, 1), // Crepuscule
    LVecBase4f(0.2, 0.2, 0.3, 1), // Night
    LVecBase4f(0.7, 0.7, 0.5, 1), // Morning
    LVecBase4f(1.0, 1.0, 1.0, 1), // Mid-day
    LVecBase4f(1.0, 0.8, 0.8, 1), // Evening
  };


  DateTime   current_time              = time_manager.GetDateTime();
  int        current_hour              = current_time.GetHour();
  int        current_minute            = current_time.GetMinute();
  int        current_second            = current_time.GetSecond();
  //int        step_duration_in_hours    = 24 / 6;
  int        step_duration;//             = DateTime::Hours(step_duration_in_hours).seconds;
  int        current_step;//              = current_hour / step_duration_in_hours;
  float      elapsed_seconds;//           = current_second + (current_minute * 60) + ((current_hour - (current_step * 4)) * 60 * 60);

  DateTime::DayLength day_length = current_time.GetDayLength();

  if (current_hour > day_length.end)
  {
    current_step    = 4;
    elapsed_seconds = current_second + (current_minute * 60) + ((current_hour - day_length.end) * 60 * 60);
    step_duration   = (24 - day_length.end) * 60 * 60;
  }
  else if (current_hour < day_length.begin)
  {
    current_step    = 0;
    elapsed_seconds = current_second + (current_minute * 60) + (current_hour * 60 * 60);
    step_duration   = day_length.begin * 60 * 60;
  }
  else if (current_hour < 12)
  {
    current_step    = 1;
    elapsed_seconds = current_second + (current_minute * 60) + ((current_hour - day_length.begin) * 60 * 60);
    step_duration   = (12 - day_length.begin) * 60 * 60;
  }
  else if (current_hour >= 12 && current_hour <= 16)
  {
    current_step    = 2;
    elapsed_seconds = current_second + (current_minute * 60) + ((current_hour - 12) * 60 * 60);
    step_duration   = 4 * 60 * 60;
  }
  else // current_hour > 16 && current_hour <= day_end
  {
    current_step    = 3;
    elapsed_seconds = current_second + (current_minute * 60) + ((current_hour - 16) * 60 * 60);
    step_duration   = (day_length.end - 16) * 60 * 60;
  }

  LVecBase4f colors                    = color_steps[current_step];
  LVecBase4f difference_with_next_step = (current_step == 4 ? color_steps[0] : color_steps[current_step + 1]) - color_steps[current_step];

  colors += (difference_with_next_step / step_duration * elapsed_seconds);
  sunlight_node->set_color(colors);
  colors.set_w(0.1);
  ambient_node->set_color(colors / 3);
}

void Sunlight::SetSunlightPositionFromTime()
{
  DateTime            current_time = time_manager.GetDateTime();
  DateTime::DayLength day_length   = current_time.GetDayLength();
  float               hour         = current_time.GetHour() + (current_time.GetMinute() / 60.f);
  bool                night        = hour < day_length.begin || hour >= day_length.end;
  float               step         = hour + (hour >= day_length.end ? -day_length.end : (hour < day_length.begin ? 24 - day_length.end : -day_length.begin));
  float               total_steps  = night ? 24 - day_length.length : day_length.length;
  step = !night ? 12 - step : step;
  //cout << "Step at " << hour << ": " << step << " because day length is " << day_length.begin << "/" << day_length.end << endl;
  float               angle        = (180.f/ total_steps) * step;
  LPoint2f            position     = solar_orbit.PointAtAngle(angle * 3.14159265f / 180.f);

  //cout << "SunLightPosition: " << position.get_x() << ',' << position.get_y() << endl;
  sunlight_nodepath.set_x(position.get_x());
  sunlight_nodepath.set_z(position.get_y());
  sunlight_nodepath.set_y(0);
  sunlight_nodepath.look_at(solar_orbit.GetPosition());
}
