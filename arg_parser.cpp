#include "arg_definition.h"
#include "utility.h"

using namespace std;
//type of input parsing fns
typedef bool (*inputFn)(string arg);

const bool ENABLE_MULTI_ARGS = false;

//parse -s
bool arg_s(string arg){
  cout << "args: " << arg << endl;
  return strToNum(arg, disk_size);
}

//parse -b
bool arg_b(string arg){
  cout << "argb: " << arg << endl;
  return strToNum(arg, blockSize);
}

//parse -f
bool arg_f(string arg){
  cout << "argf: " << arg << endl;
  input_files = arg;
  return true;
}

//parse -d
bool arg_d(string arg){
  cout << "argd: " << arg << endl;
  input_directories = arg;
  return true;
}

//parse all configured args
bool parseArgs(int start, int argc, char ** argv){
  unordered_map<string, inputFn> cmd_map;
  unordered_map<string, inputFn>::const_iterator it, end;  
  cmd_map["-f"] = &arg_f;
  cmd_map["-s"] = &arg_s;
  cmd_map["-d"] = &arg_d;
  cmd_map["-b"] = &arg_b;

  end = cmd_map.end();
  
  string cmds = "";
  string key = "";
  for(int i = start; i < argc; ++i){
    if(key.size() == 0){
      it = cmd_map.find(argv[i]);
      if(it == end){
	return false;
      }else{
	key = argv[i];
	cmds = "";
      }
    }else{
      it = cmd_map.find(argv[i]);
      if(it == end){
	if(!ENABLE_MULTI_ARGS){
	  if(cmds.size())
	    return false;
	}
	if(cmds.size())
	  cmds += " ";
	cmds += argv[i];
      }else{
	if(!cmd_map[key](cmds))
	  return false;
	key = argv[i];
	cmds = "";
      }
    }
  }
  
  if(key.size()){
    cmd_map[key](cmds);
  }
  
  return true;
}

