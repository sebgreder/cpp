#include "Configuration.h"

int main()
{
 Configuration c{"/home/slrx/Informatique/c++/test.config"};
 c.print("var3");
 c.print("var2","noname");
}


