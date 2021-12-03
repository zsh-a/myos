#include "io.h"
#include "types.h"
#include "x86.h"
extern "C" void printf(const char *str) {
  u16 *monitor_io_memory = reinterpret_cast<u16 *>(
      0xb8000);  //注意！重点来啦！0xb8000内存地址是显示器地址，往这里写数据就直接能够输出到屏幕上
  for (int i = 0; str[i] != '\0'; ++i) {
    //写入字符串，取或0xff00的意思是我们需要把屏幕高四位拉低，否则就是黑色的字体，黑色的字体黑色的屏幕是啥也看不到的
    monitor_io_memory[i] = (monitor_io_memory[i] & 0xff00) | str[i];
  }
}

extern "C" void kernel_main() {
  init_gdt();
  printf("Hello World");
  while (1)
    ;
}
