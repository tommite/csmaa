#ifndef __GUI_MAINWINDOW_H__
#define __GUI_MAINWINDOW_H__

#include <cstdio>
#include <gtkmm/notebook.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>
#include <gtkmm/comboboxtext.h>
#include <glibmm/ustring.h>
#include <gtkmm/table.h>
#include <gtkmm/radiobuttongroup.h>
#include <gtkmm/radiobutton.h>
#include "globals.h"
#include <cassert>
#include <vector>
#include <gtkmm/entry.h>
#include <stdlib.h>
#include <limits.h>
#include <string>
#include "measurementstable.h"
#include "cardinalpreferencestable.h"
#include "ordinalpreferencestable.h"
#include "inputcriteriontable.h"
#include "resultstable.h"
#include "../electre/electre.h"
#include <gtkmm/progressbar.h>
#include <glibmm/thread.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <fstream>
#include <gtkmm/messagedialog.h>
#include "simulation.h"
#include "exactpreferencestable.h"
#include <gtkmm/scrolledwindow.h>
//#include <gtkmm/tooltips.h>
#include <gtkmm/main.h>

#include <gtkmm/viewport.h>
class MainWindow : public Gtk::Window
{
 public:
  MainWindow();
  virtual ~MainWindow();

 protected:

  // enums
  enum {
    SMAA2 = 0,
    SMAATRI,
    SMAAIII,
    SMAA3,
  };

  int get_no_alts() const;
  int get_no_cats() const;
  int get_no_crit() const;

  // general variables
  int method;
  bool model_changed;
  int old_page;
  bool altname_changed;
  bool catname_changed;
  bool input_cardpref_on;
  bool input_ordpref_on;
  bool input_expref_on;

  // signal handlers
  void on_forw_button_clicked();
  void on_back_button_clicked();
  void on_switch_page(GtkNotebookPage *page, guint i);
  void on_altname_changed();
  void on_catname_changed();
  void on_ip_card_button_clicked();
  void on_ip_ord_button_clicked();
  void on_ip_ex_button_clicked();
  void on_but_execute_clicked();
  void on_menu_open();
  void on_menu_save();
  void on_menu_save_as();
  void on_menu_quit();
  void on_menu_about();
  bool on_quit_proxy(void *v);
  void on_iuc_but_clicked(double am);
  void on_ipuc_but_clicked(double am);

  void info_dialog(std::string text);
  inline void info_dialog(const char *text)
  { info_dialog(std::string(text)); }

  // method activators
  void activate_method_smaa2();
  void activate_method_smaaTRI();
  void activate_method_smaaIII();
  void activate_method_smaa3();

  //misc funcs
  /* updates the GUI according to the number of alternatives etc */
  void update_nos();
  /* updates the GUI according to the names of the alternatives */
  void update_altnames();
  /* updates the criteria information in the model */
  void update_criteriainfo();

  void update_catnames();

  Glib::Dispatcher signal_update_runstatus;
  void update_runstatus();

  void create_criterion_input(int index);

  void simulation_run();

  void save_model(const string & fname) throw (string);
  void load_model(const string & fname) throw (string);

  void sensitivize_tabs(bool sensitive);
  void clear_changed_info();
  void set_changed()
    { model_changed = true; }

  // GUI symbols
  Gtk::Notebook tabs;
  Gtk::VBox vbox;

  // menu
  Glib::RefPtr<Gtk::UIManager> uimanager;
  Glib::RefPtr<Gtk::ActionGroup> menugrp;

  // signal handlers
  sigc::connection switch_page_conn;

  // forward and backward buttons and their container
  Gtk::HBox box_downbuttons;
  Gtk::Button back_button;
  Gtk::Button forw_button;

  // tab_selectmethod
  Gtk::Frame tab_selectmethod;
  Gtk::Label l_selectmethod;
  Gtk::RadioButton sm_rb_smaa2;
  Gtk::RadioButton sm_rb_smaaTRI;
  Gtk::RadioButton sm_rb_smaaIII;
  Gtk::RadioButton sm_rb_smaa3;
  Gtk::RadioButtonGroup sm_group;
  Gtk::VBox sm_vbox;

  // tab_selectnos
  Gtk::Frame tab_selectnos;
  Gtk::Table tab_sn_table;
  Gtk::ComboBoxText cb_selectalts;
  Gtk::ComboBoxText cb_selectcrit;
  Gtk::ComboBoxText cb_selectcats;
  Gtk::Label label_no_alts;
  Gtk::Label label_no_crit;
  Gtk::Label label_no_cats;

  // tab_inputaltnames
  Gtk::Frame tab_inputaltnames;
  Gtk::VBox tab_ian_vbox;
  //  Gtk::Label l_inputaltnames;
  Gtk::ScrolledWindow scroll_ian;
  vector<Gtk::Entry *> v_entry_altsnames;

  // tab_inputcatnames
  Gtk::Frame tab_inputcatnames;
  Gtk::VBox tab_icatsn_vbox;
  Gtk::Label lab_catinp;
  Gtk::ScrolledWindow scroll_icats;
  vector<Gtk::Entry *> v_entry_catsnames;

  // tab_inputcriteria
  Gtk::Frame tab_inputcriteria;
  InputCriterionTable input_c_table;
  Gtk::ScrolledWindow scroll_inputc;

  // tab_profiles
  Gtk::Label iprof_label;
  Gtk::VBox iprof_box;
  Gtk::Frame tab_inputprofiles;
  MeasurementsTable iprof_table;
  Gtk::ScrolledWindow scroll_iprof;

  // tab_inputmeasurements
  Gtk::Label im_label;
  Gtk::VBox im_box;
  Gtk::Frame tab_inputmeasurements;
  MeasurementsTable im_table;
  Gtk::ScrolledWindow scroll_im;

  // tab_inputuncertainties
  Gtk::Frame tab_inputuncertainties;
  Gtk::Label iuc_label;
  Gtk::VBox iuc_box;
  MeasurementsTable iuc_table;
  Gtk::ScrolledWindow scroll_iuc;
  Gtk::HBox iuc_set_box;
  Gtk::Label iuc_set_label;
  Gtk::Button iuc_0_but;
  Gtk::Button iuc_5_but;
  Gtk::Button iuc_10_but;
  Gtk::Button iuc_20_but;

  // tab_inputprofuncertainties
  Gtk::Frame tab_inputprofuncertainties;
  Gtk::Label ipuc_label;
  Gtk::VBox ipuc_box;
  MeasurementsTable ipuc_table;
  Gtk::ScrolledWindow scroll_ipuc;
  Gtk::HBox ipuc_set_box;
  Gtk::Label ipuc_set_label;
  Gtk::Button ipuc_0_but;
  Gtk::Button ipuc_5_but;
  Gtk::Button ipuc_10_but;
  Gtk::Button ipuc_20_but;

  // tab_inputpreferences
  Gtk::ScrolledWindow scroll_inppref;
  Gtk::Frame tab_inputpreferences;
  Gtk::Table ip_table;
  Gtk::Button ip_card_button;
  Gtk::Button ip_ord_button;
  Gtk::Button ip_ex_button;
  CardinalPreferencesTable ip_card_table;
  OrdinalPreferencesTable ip_ord_table;
  ExactPreferencesTable ip_exact_table;

  // tab_execute
  Gtk::Frame tab_execute;
  Gtk::HBox exec_up_hbox;
  Gtk::Label exec_up_label;
  Gtk::ComboBoxText exec_up_box;
  Gtk::VBox exec_vbox;
  ResultsTable trires_table;
  Gtk::Button but_execute;
  Gtk::ProgressBar exec_progbar;
  Gtk::HBox exec_up_rulebox;
  Gtk::Label exec_rulelabel;
  Gtk::ComboBoxText exec_rulechooser;
  Gtk::HBox exec_up_lambdabox;
  Gtk::Label exec_lambdaminlabel;
  Gtk::Label exec_lambdamaxlabel;
  NumberEntry exec_lambda_min;
  NumberEntry exec_lambda_max;
  Gtk::ScrolledWindow result_scroll_TRI;
  Gtk::ScrolledWindow result_scroll_III;
  Gtk::ScrolledWindow result_scroll_3;
  Gtk::Label rankacc_label_3;
  Gtk::Label catacc_label;
  Gtk::VBox result_box_TRI;
  Gtk::VBox result_box_III;
  Gtk::VBox result_box_3;
  Gtk::Label rankacc_label;
  ResultsTable smaa3_rankacc_table;
  ResultsTable smaaIII_rankacc_table;
  Gtk::Label pwwin_label;
  ResultsTable smaaIII_pwwin_table;
  Gtk::Label incomp_label;
  ResultsTable smaaIII_incomp_table;

  // thread stuff
  Glib::Thread *th;
  bool run;
  int curiter;
  int iters;

  //filename
  std::string filename;

  int no_alts, no_crit, no_cats;
  //  Gtk::Tooltips tips;

};


#endif
