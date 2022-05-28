#include "../include/Game.h"

int main(int argc, char* argv[]) {
   std::string root_path;
   for (int idx = 1; idx < argc; ++idx)
   {
      std::string arg = argv[idx];
      if (arg.compare(0, 8, "RootPath") == 0)
      {
         root_path = arg.substr(9);
      }
   }

   Game game(root_path);
   game.run();
   return 0;
}