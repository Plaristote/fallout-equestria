#include "level/sunlight.hpp"
#include "options.hpp"

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

  // TODO implement SetLightOnModels and remove this
  sun_root.set_light(sunlight_nodepath, 6);
  sun_root.set_light(ambient_nodepath, 5);
}

Sunlight::~Sunlight()
{
  world.floors_node.set_light_off(sunlight_nodepath);
  world.floors_node.set_light_off(ambient_nodepath);
  world.floors_node.clear_light();
  ambient_nodepath.remove_node();
  sunlight_nodepath.remove_node();
}

void Sunlight::InitializeSun()
{
  NodePath sun_root = world.floors_node;

  if (static_sunlight.is_null())
    static_sunlight = new DirectionalLight("sunlight");
  sunlight_node     = static_sunlight;
  sunlight_nodepath = sun_root.attach_new_node(sunlight_node);
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
  sunlight_node->get_lens()->set_near_far(10.f, 1200.f);
  sunlight_node->get_lens()->set_film_size(GetFilmSize());
}

unsigned int Sunlight::GetShadowBufferSize(void)
{
  unsigned int shadow_buffer_size = 128;
  unsigned int graphics_quality = OptionsManager::Get()["graphics-quality"];

  shadow_buffer_size *= 2 * (graphics_quality + 1);
  return (shadow_buffer_size);
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
  if (!sunlight_nodepath.is_empty())
  {
    SetLightColorFromTime();
    SetSunlightPositionFromTime();
  }
  ScheduledTask::Run();
}

void Sunlight::SetLightColorFromTime(void)
{
  LVecBase4f color_steps[6]            = {
    LVecBase4f(0.2, 0.2, 0.5, 1), // 00h00
    LVecBase4f(0.2, 0.2, 0.3, 1), // 04h00
    LVecBase4f(0.7, 0.7, 0.5, 1), // 08h00
    LVecBase4f(1.0, 1.0, 1.0, 1), // 12h00
    LVecBase4f(1.0, 0.8, 0.8, 1), // 16h00
    LVecBase4f(0.4, 0.4, 0.6, 1)  // 20h00
  };
  DateTime   current_time              = time_manager.GetDateTime();
  int        current_hour              = current_time.GetHour();
  int        current_minute            = current_time.GetMinute();
  int        current_second            = current_time.GetSecond();
  int        step_duration_in_hours    = 24 / 6;
  int        step_duration             = DateTime::Hours(step_duration_in_hours).seconds;
  int        current_step              = current_hour / step_duration_in_hours;
  float      elapsed_seconds           = current_second + (current_minute * 60) + ((current_hour - (current_step * 4)) * 60 * 60);
  LVecBase4f colors                    = color_steps[current_step];
  LVecBase4f difference_with_next_step = (current_step == 5 ? color_steps[0] : color_steps[current_step + 1]) - color_steps[current_step];

  colors += (difference_with_next_step / step_duration * elapsed_seconds);
  sunlight_node->set_color(colors);
  colors.set_w(0.1);
  ambient_node->set_color(colors / 3);
}

void Sunlight::SetSunlightPositionFromTime()
{
  DateTime current_time = time_manager.GetDateTime();
  float    step         = current_time.GetHour() / 12 + (current_time.GetMinute() / 60.f);
  float    angle        = ((180.f / 120.f) / 12.f) * step;
  LPoint2f position     = solar_orbit.PointAtAngle(angle);

  //sunlight_nodepath.set_z(10);
  //sunlight_nodepath.set_y(0);
  /*cout << "SunLightPosition: " << position.get_x() << ',' << position.get_y() << endl;
  sunlight_nodepath.set_x(0);
  sunlight_nodepath.set_z(position.get_x());
  sunlight_nodepath.set_y(position.get_y());
  sunlight_nodepath.look_at(solar_orbit.GetPosition());*/
}
