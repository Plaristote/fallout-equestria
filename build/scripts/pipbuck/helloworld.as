
// 0: file list
// 1: file text
// 2: file sound
int         current_interface = 0;
string      current_file;
RmlElement@ myroot;

bool start(Data data)
{
  Write("Helloworld Started");
  return (true);
}

void exit(Data data)
{
  current_interface = 0;
  current_file      = "";
  Write("Helloworld Exited");
}

void focused(RmlElement@ root, Data data)
{
  Write("FileManager Focused");
  string rml;

  @myroot = @root;
  rml     = "<h1>File Manager 2000</h1>";
  if (current_interface == 0)
    rml += display_file_list(data);
  else if (current_interface == 1)
    rml += display_file_text(data);
  root.SetInnerRML(rml);
  Write("InnerRML Set");
  if (current_interface == 0)
    events_file_list(data);
  Write("Focused ended");
}

string display_file_list(Data data)
{
  Data   files   = data["Pipbuck"]["Files"];
  int    n_files = files.Count();
  int    it      = 0;
  string rml     = "";

  rml += "<div id='file_list'>";
  while (it < n_files)
  {
    Data   file = files[it];
    string icon = "../textures/pipbuck/icons/" + file["type"].AsString() + ".png";

    rml += "<datagrid id='file-" + file.Key() + "'>\n";
    rml += "  <col width='20%'><img src='" + icon + "' /></col>\n";
    rml += "  <col width='80%'>" + file["header"].AsString() + "</col>\n";
    rml += "</datagrid>\n";
    it  += 1;
  }
  rml += "</div>";
  return (rml);
}

void   events_file_list(Data data)
{
  Write("Executing events_file_list");
  Data   files   = data["Pipbuck"]["Files"];
  int    n_files = files.Count();
  int    it      = 0;

  while (it < n_files)
  {
    Data        file = files[it];
    RmlElement@ elem = myroot.GetElementById("file-" + file.Key());

    if (@elem != null)
    {
      elem.ClearEventListeners();
      elem.AddEventListener("click", "file_opened");
    }
    else
      Write("[Pipbuck][FileManager][Error] No element with id 'file-" + file.Key() + "'");
    it += 1;
  }
}

void   file_opened(Data data, RmlElement@ current_element, string event)
{
  string rml;
  Data   files   = data["Pipbuck"]["Files"];
  int    it      = 0;
  int    n_files = files.Count();
  string elem_id = current_element.GetId();

  Write("[Pipbuck][FileManager] Loading file " + elem_id);
  while (it < n_files)
  {
    Data file = files[it];

    if ("file-" + file.Key() == elem_id)
    {
      current_interface = 1;
      current_file      = file.Key();
      break ;
    }
    it += 1;
  }
  current_element.ClearEventListeners();
  rml = display_file_text(data);
  myroot.SetInnerRML(rml);
}

string display_file_text(Data data)
{
  Data   file = data["Pipbuck"]["Files"][current_file];
  string rml  = "";

  rml += "<div id='file-header'>" + file["header"].AsString() + "</div>";
  rml += "<div id='file-context'>" + file["content"].AsString() + "</div>";
  return (rml);
}

void unfocused(Data data)
{
  Write("Helloworld Unfocused");
}

int main(RmlElement@ root, Data data)
{
  return (0);
}

int daemon(Data data)
{
  return (0);
}

