#ifndef __GUI_MAIN_H__
#define __GUI_MAIN_H__

#include <gtkmm.h>
#include <glibmm/thread.h>
#include "globals.h"
#include "mainwindow.h"
#include <locale>
#include <clocale>

int main(int argc, char *argv[])
{
  // set locales
  setlocale(LC_ALL, "C");
  std::locale::global(std::locale("C"));

  Glib::thread_init();

  Gtk::Main kit(argc, argv);

  MainWindow window;
  Gtk::Main::run(window);

  return 0;
}




#endif
