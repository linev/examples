#include <string>
#include <fstream>
#include <streambuf>
#include <stdio.h>

#include "TCanvas.h"
#include "TBufferJSON.h"

void canv() {
   std::ifstream t("canv.json");
   std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
   TCanvas *c3 = nullptr;
   TBufferJSON::FromJSON(c3, str.c_str());
   if (c3) { 
      printf("Get class %s\n", c3->ClassName());  
      c3->SetName("c3"); 
      c3->Draw(); 
   } 
}

