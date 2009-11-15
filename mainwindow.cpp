#include "mainwindow.h"

bool MainWindow::on_quit_proxy(void *v)
{
  on_menu_quit();
  return true;
}

MainWindow::MainWindow()
  : 
    tab_sn_table(2,3),
    input_c_table(3,3),
    iprof_table(3,2,false),
    im_table(3,3,false, false, false),
    iuc_table(3,3,false, true),
    ipuc_table(3,3,false, true),
    ip_card_table(3),
    ip_ord_table(3),
    ip_exact_table(3),
    trires_table(3, 3),
    smaa3_rankacc_table(3,3),
    smaaIII_rankacc_table(3, 3),    
    smaaIII_pwwin_table(3, 3),
    smaaIII_incomp_table(3,3)
{
  char textbuf[10];
  set_title(APPNAME);
  set_border_width(5);
  set_default_size(600, 400);
  run = false;
  no_alts = 3;
  no_crit = 3;
  no_cats = 3;

  input_cardpref_on = false;
  input_ordpref_on = false;
  input_expref_on = false;

  scroll_ian.set_shadow_type(Gtk::SHADOW_NONE);
  scroll_icats.set_shadow_type(Gtk::SHADOW_NONE);
  scroll_inputc.set_shadow_type(Gtk::SHADOW_NONE);
  scroll_iprof.set_shadow_type(Gtk::SHADOW_NONE);
  scroll_im.set_shadow_type(Gtk::SHADOW_NONE);
  scroll_iuc.set_shadow_type(Gtk::SHADOW_NONE);
  scroll_ipuc.set_shadow_type(Gtk::SHADOW_NONE);
  scroll_inppref.set_shadow_type(Gtk::SHADOW_NONE);
  result_scroll_TRI.set_shadow_type(Gtk::SHADOW_NONE);
  result_scroll_III.set_shadow_type(Gtk::SHADOW_NONE);

  signal_delete_event().connect(sigc::mem_fun(*this, &MainWindow::on_quit_proxy));
  
  signal_update_runstatus.connect(sigc::mem_fun(*this, &MainWindow::update_runstatus));

  add (vbox);
  old_page = 0;

  // create menu
  menugrp = Gtk::ActionGroup::create();
  menugrp->add(Gtk::Action::create("FileMenu", "File"));
  menugrp->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN), sigc::mem_fun(*this, &MainWindow::on_menu_open));
  menugrp->add(Gtk::Action::create("FileSave", Gtk::Stock::SAVE), sigc::mem_fun(*this, &MainWindow::on_menu_save));
  menugrp->add(Gtk::Action::create("FileSaveAs", Gtk::Stock::SAVE_AS), sigc::mem_fun(*this, &MainWindow::on_menu_save_as));
  menugrp->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT), sigc::mem_fun(*this, &MainWindow::on_menu_quit));

  menugrp->add(Gtk::Action::create("HelpMenu", "Help"));
  menugrp->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT), sigc::mem_fun(*this, &MainWindow::on_menu_about));  

  uimanager = Gtk::UIManager::create();
  uimanager->insert_action_group(menugrp);
  add_accel_group(uimanager->get_accel_group());

  try {
    Glib::ustring ui_info = 
      "<ui>" 
      "  <menubar name='MenuBar'>"
      "    <menu action='FileMenu'>"
      "      <menuitem action='FileOpen'/>"
      "      <menuitem action='FileSave'/>"
      "      <menuitem action='FileSaveAs'/>"
      "      <menuitem action='FileQuit'/>"
      "    </menu>"
      "    <menu action='HelpMenu'>"
      "      <menuitem action='HelpAbout'/>"
      "    </menu>"      
      "  </menubar>"
      "</ui>";
    uimanager->add_ui_from_string(ui_info);
  }
  catch (const Glib::Error &e)
    {
      std::cerr << "Building menus failed: " <<e.what() << "\n";
      exit(1);
    }

  Gtk::Widget *pmenbar = uimanager->get_widget("/MenuBar");
  if (pmenbar)
    vbox.pack_start(*pmenbar, Gtk::PACK_SHRINK);
  else
    {
      std::cerr << "Error getting menu." << std::endl;
      exit(1);
    }

  tabs.set_border_width(5);

  vbox.pack_start(tabs);

  exec_progbar.set_text("Inactive");
  exec_progbar.set_fraction(0.0);

  // create tab_selectmethod
  sm_rb_smaa2.set_label(Glib::ustring("SMAA-2"));
  sm_rb_smaaTRI.set_label(Glib::ustring("SMAA-TRI"));
  sm_rb_smaaIII.set_label(Glib::ustring("SMAA-III"));
  sm_rb_smaa3.set_label(Glib::ustring("SMAA-3"));
  sm_rb_smaa2.set_group(sm_group);
  sm_rb_smaaTRI.set_group(sm_group);
  sm_rb_smaaIII.set_group(sm_group);
  sm_rb_smaa3.set_group(sm_group);
  l_selectmethod.set_text ("Select method");
  sm_vbox.pack_start(l_selectmethod, Gtk::PACK_SHRINK, 5);
  //  sm_vbox.pack_start(sm_rb_smaa2, Gtk::PACK_SHRINK, 5);
  sm_vbox.pack_start(sm_rb_smaaTRI, Gtk::PACK_SHRINK, 5);
  sm_vbox.pack_start(sm_rb_smaaIII, Gtk::PACK_SHRINK, 5);
  sm_vbox.pack_start(sm_rb_smaa3, Gtk::PACK_SHRINK, 5);
  tab_selectmethod.set_border_width(5);
  tab_selectmethod.add(sm_vbox);
  /*
  tips.set_tip(sm_rb_smaa2, "Activate SMAA-2 method");
  tips.set_tip(sm_rb_smaaTRI, "Activate SMAA-TRI method");
  tips.set_tip(sm_rb_smaaIII, "Activate SMAA-III method");
  */

  // create tab_selectnos
  label_no_alts.set_text("Select the number of alternatives");
  label_no_crit.set_text("Select the number of criteria");
  label_no_cats.set_text("Select the number of categories");
  tab_selectnos.set_border_width(5);
  tab_selectnos.add(tab_sn_table);  
  tab_sn_table.attach(label_no_alts, 0, 1, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  tab_sn_table.attach(label_no_crit, 0, 1, 1, 2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  tab_sn_table.attach(label_no_cats, 0, 1, 2, 3, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  tab_sn_table.attach(cb_selectalts, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  tab_sn_table.attach(cb_selectcrit, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  tab_sn_table.attach(cb_selectcats, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  for (int i=0;i<MAXALTS;i++)
    {
      sprintf (textbuf, "%d", i+1);
      cb_selectalts.append_text(Glib::ustring(textbuf));
    }
  for (int i=1;i<MAXCATS;i++)
    {
      sprintf (textbuf, "%d", i+1);
      cb_selectcats.append_text(Glib::ustring(textbuf));
    }
  for (int i=0;i<MAXCRIT;i++)
    {
      sprintf (textbuf, "%d", i+1);
      cb_selectcrit.append_text(Glib::ustring(textbuf));
    }

  cb_selectalts.set_active(get_no_alts()-1);
  cb_selectcrit.set_active(get_no_crit()-1);
  cb_selectcats.set_active(get_no_cats()-2);

  cb_selectalts.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::update_nos));
  cb_selectcrit.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::update_nos));
  cb_selectcats.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::update_nos));

#ifdef DEBUG
  printf ("tab_inputnos created...\n");
#endif

  im_table.resize(get_no_crit(),get_no_alts());
  iuc_table.resize(get_no_crit(),get_no_alts());
  ipuc_table.resize(get_no_crit(),get_no_alts()-1);

  // create tab_inputaltnames
  tab_inputaltnames.set_border_width(5);
  v_entry_altsnames.resize(get_no_alts());
  //  l_inputaltnames.set_text("Input names for alternatives");
  //  tab_ian_vbox.pack_start(l_inputaltnames, Gtk::PACK_SHRINK, 5);
  for (unsigned int i=0;i<v_entry_altsnames.size();i++)
	{
	  char textbuf[20];
	  sprintf (textbuf, "Alternative %d", i+1);
	  v_entry_altsnames[i] = new Gtk::Entry();
	  v_entry_altsnames[i]->set_width_chars(ENTRYWIDTH);
	  v_entry_altsnames[i]->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_altname_changed));
	  v_entry_altsnames[i]->set_max_length(MAXNAMELENGTH);
	  v_entry_altsnames[i]->set_text(Glib::ustring(textbuf));
	  tab_ian_vbox.pack_start(*v_entry_altsnames[i], Gtk::PACK_SHRINK, 5);
	  im_table.set_alternative_name(i, textbuf);
	  iuc_table.set_alternative_name(i, textbuf);
	  trires_table.set_alternative_name(i, textbuf);
	  smaaIII_rankacc_table.set_alternative_name(i, textbuf);
	  smaa3_rankacc_table.set_alternative_name(i, textbuf);
	  smaaIII_pwwin_table.set_alternative_name(i, textbuf);
	  smaaIII_pwwin_table.set_category_name(i, textbuf);
	  smaaIII_incomp_table.set_alternative_name(i, textbuf);
	  smaaIII_incomp_table.set_category_name(i, textbuf);
	  sprintf (textbuf, "%d", i+1);
	  smaaIII_rankacc_table.set_category_name(i, textbuf);
	  smaa3_rankacc_table.set_category_name(i, textbuf);
	}
  scroll_ian.set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  tab_inputaltnames.add(scroll_ian);
  scroll_ian.add(tab_ian_vbox);
#ifdef DEBUG
  printf ("tab_inputaltnames created...\n");
#endif

  // create tab_inputcatnames
  lab_catinp.set_text("Input category names in ascending order, e.g. FIRST category is the WORST one.");
  tab_inputcatnames.set_border_width(5);
  v_entry_catsnames.resize(get_no_cats());
  tab_icatsn_vbox.pack_start(lab_catinp, Gtk::PACK_SHRINK, 5, 5);
  for (int i=0;i<get_no_cats();i++)
	{
	  char textbuf[20];
	  sprintf (textbuf, "Category %d", i+1);
	  v_entry_catsnames[i] = new Gtk::Entry();
	  v_entry_catsnames[i]->set_width_chars(ENTRYWIDTH);
	  v_entry_catsnames[i]->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_catname_changed));
	  v_entry_catsnames[i]->set_max_length(MAXNAMELENGTH);
	  v_entry_catsnames[i]->set_text(Glib::ustring(textbuf));
	  tab_icatsn_vbox.pack_start(*v_entry_catsnames[i], Gtk::PACK_SHRINK, 5, 5);
	  trires_table.set_category_name(i, textbuf);
	}
  scroll_icats.set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  scroll_icats.add(tab_icatsn_vbox);
  tab_inputcatnames.add(scroll_icats);
#ifdef DEBUG
  printf ("tab_inputcatnames created...\n");
#endif

  // create tab_inputcriteria
  tab_inputcriteria.set_border_width(5);
  scroll_inputc.add(input_c_table);
  tab_inputcriteria.add(scroll_inputc);
  scroll_inputc.set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  for (int i=0;i<get_no_crit();i++)
    {
      Glib::ustring nl = input_c_table.get_name(i);
      crittype typ = input_c_table.get_type(i);

      im_table.set_criterion_name(i, nl);
      im_table.set_criterion_type(i, typ);
      
      iuc_table.set_criterion_name(i, nl);
      iuc_table.set_criterion_type(i, typ);
      
      ip_card_table.set_criterion_name(i, nl);
      ip_ord_table.set_criterion_name(i, nl);
      ip_exact_table.set_criterion_name(i, nl);

      iprof_table.set_criterion_name(i, nl);
      iprof_table.set_criterion_type(i, typ);

      ipuc_table.set_criterion_name(i, nl);
      ipuc_table.set_criterion_type(i, typ);
    }
#ifdef DEBUG
  printf ("tab_inputcriteria created...\n");
#endif

  for (int i=0;i<get_no_cats()-1;i++)
    {
      sprintf (textbuf, "Profile %d", (i+1));
      iprof_table.set_alternative_name(i, textbuf);
      ipuc_table.set_alternative_name(i, textbuf);
    }
  // create tab_inputprofiles
  tab_inputprofiles.set_border_width(5);
  iprof_label.set_text("Input measurements for the profiles.\nFor uniform distributed cardinal real: mean of the interval.\nFor uniform distributed cardinal discrete: minimum value.\nFor Gaussian distributed: the mean.");
  iprof_box.pack_start(iprof_label, Gtk::PACK_SHRINK, 5);
  iprof_box.pack_start(iprof_table, Gtk::PACK_SHRINK, 5);
  scroll_iprof.add(iprof_box);
  tab_inputprofiles.add(scroll_iprof);
  scroll_iprof.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
#ifdef DEBUG
  printf ("tab_inputprofiles created...\n");
#endif

  // create tab_inputmeasurements
  tab_inputmeasurements.set_border_width(5);
  im_label.set_text("Input measurements for the alternatives.\nFor ordinal criteria: the rank.\nFor uniform distributed cardinal real: mean of the interval.\nFor uniform distributed cardinal discrete: minimum value.\nFor Gaussian distributed: the mean.");
  im_box.pack_start(im_label, Gtk::PACK_SHRINK, 5);
  im_box.pack_start(im_table, Gtk::PACK_SHRINK, 5);
  scroll_im.add(im_box);
  scroll_im.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  tab_inputmeasurements.add(scroll_im);
#ifdef DEBUG
  printf ("tab_inputmeasurements created...\n");
#endif

  // create tab_inputuncertainties
  tab_inputuncertainties.set_border_width(5);
  iuc_label.set_text("Input alternatives criteria uncertainties.\nFor uniform distributed cardinal real: +- around the mean.\nFor uniform distributed cardinal discrete: maximum value.\nFor Gaussian distributed: standard deviation around the mean.");
  iuc_set_label.set_text("Set uncertainties to portions of measurements:");
  iuc_5_but.set_label("5%");
  iuc_10_but.set_label("10%");
  iuc_20_but.set_label("20%");
  iuc_0_but.set_label("Reset all zero");
  iuc_set_box.pack_start(iuc_set_label, Gtk::PACK_SHRINK, 5);
  iuc_set_box.pack_start(iuc_5_but, Gtk::PACK_SHRINK, 5);
  iuc_set_box.pack_start(iuc_10_but, Gtk::PACK_SHRINK, 5);
  iuc_set_box.pack_start(iuc_20_but, Gtk::PACK_SHRINK, 5);  
  iuc_set_box.pack_start(iuc_0_but, Gtk::PACK_SHRINK, 5);  

  iuc_0_but.signal_clicked().connect(sigc::bind<double>(sigc::mem_fun(*this, &MainWindow::on_iuc_but_clicked), 0.0));
  iuc_5_but.signal_clicked().connect(sigc::bind<double>(sigc::mem_fun(*this, &MainWindow::on_iuc_but_clicked), 0.05));
  iuc_10_but.signal_clicked().connect(sigc::bind<double>(sigc::mem_fun(*this, &MainWindow::on_iuc_but_clicked), 0.1));
  iuc_20_but.signal_clicked().connect(sigc::bind<double>(sigc::mem_fun(*this, &MainWindow::on_iuc_but_clicked), 0.2));


  iuc_box.pack_start(iuc_label, Gtk::PACK_SHRINK, 5);
  iuc_box.pack_start(iuc_set_box, Gtk::PACK_SHRINK, 5);
  iuc_box.pack_start(iuc_table, Gtk::PACK_SHRINK, 5);
  scroll_iuc.add(iuc_box);
  scroll_iuc.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  tab_inputuncertainties.add(scroll_iuc);
#ifdef DEBUG
  printf ("tab_inputuncertainties created...\n");
#endif

  // create tab_inputprofuncertainties
  tab_inputprofuncertainties.set_border_width(5);
  ipuc_label.set_text("Input profiles criteria uncertainties.\nFor uniform distributed cardinal real: +- around the mean.\nFor uniform distributed discrete: maximum value.\nFor Gaussian distributed: standard deviation around the mean.");

  ipuc_set_label.set_text("Set uncertainties to portions of measurements:");
  ipuc_5_but.set_label("5%");
  ipuc_10_but.set_label("10%");
  ipuc_20_but.set_label("20%");
  ipuc_0_but.set_label("Reset all zero");
  ipuc_set_box.pack_start(ipuc_set_label, Gtk::PACK_SHRINK, 5);
  ipuc_set_box.pack_start(ipuc_5_but, Gtk::PACK_SHRINK, 5);
  ipuc_set_box.pack_start(ipuc_10_but, Gtk::PACK_SHRINK, 5);
  ipuc_set_box.pack_start(ipuc_20_but, Gtk::PACK_SHRINK, 5);  
  ipuc_set_box.pack_start(ipuc_0_but, Gtk::PACK_SHRINK, 5);  

  ipuc_0_but.signal_clicked().connect(sigc::bind<double>(sigc::mem_fun(*this, &MainWindow::on_ipuc_but_clicked), 0.0));
  ipuc_5_but.signal_clicked().connect(sigc::bind<double>(sigc::mem_fun(*this, &MainWindow::on_ipuc_but_clicked), 0.05));
  ipuc_10_but.signal_clicked().connect(sigc::bind<double>(sigc::mem_fun(*this, &MainWindow::on_ipuc_but_clicked), 0.1));
  ipuc_20_but.signal_clicked().connect(sigc::bind<double>(sigc::mem_fun(*this, &MainWindow::on_ipuc_but_clicked), 0.2));

  ipuc_box.pack_start(ipuc_label, Gtk::PACK_SHRINK, 5);
  ipuc_box.pack_start(ipuc_set_box, Gtk::PACK_SHRINK, 5);
  ipuc_box.pack_start(ipuc_table, Gtk::PACK_SHRINK, 5);
  scroll_ipuc.add(ipuc_box);
  scroll_ipuc.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  tab_inputprofuncertainties.add(scroll_ipuc);
#ifdef DEBUG
  printf ("tab_inputprofuncertainties created...\n");
#endif

  // create tab_inputpreferences
  ip_table.resize(3, 2);
  ip_card_button.set_label("Add cardinal (upper and lower bound) preferences");
  ip_ord_button.set_label("Add ordinal (ranking) preferences");
  ip_ex_button.set_label("Add exact preferences");
  ip_card_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_ip_card_button_clicked));
  ip_ord_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_ip_ord_button_clicked));
  ip_ex_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_ip_ex_button_clicked));
  tab_inputpreferences.set_border_width(5);
  scroll_inppref.add(ip_table);
  scroll_inppref.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  tab_inputpreferences.add(scroll_inppref);
  ip_table.attach(ip_ex_button, 0, 1, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  ip_table.attach(ip_card_button, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  ip_table.attach(ip_ord_button, 2, 3, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
#ifdef DEBUG
  printf ("tab_inputpreferences created...\n");
#endif

  // create tab_execute
  result_scroll_III.set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  result_scroll_TRI.set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  result_scroll_3.set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  but_execute.set_label(Glib::ustring("Execute"));
  but_execute.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_but_execute_clicked));
  tab_execute.set_border_width(5);
  exec_up_label.set_text("Select number of iterations");
  exec_up_box.append_text("10000");
  exec_up_box.append_text("20000");
  exec_up_box.append_text("50000");
  exec_up_box.set_active(0);
  exec_up_box.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::set_changed));
  exec_up_hbox.pack_start(exec_up_label, Gtk::PACK_SHRINK, 5);
  exec_up_hbox.pack_start(exec_up_box, Gtk::PACK_SHRINK, 5);
  exec_up_hbox.pack_start(but_execute, Gtk::PACK_SHRINK, 5);
  exec_up_hbox.pack_start(exec_progbar, Gtk::PACK_SHRINK, 20);

  exec_rulelabel.set_text("Assignment rule");
  exec_rulechooser.append_text("Pessimistic");
  exec_rulechooser.append_text("Optimistic");
  exec_rulechooser.set_active(0);
  exec_rulechooser.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::set_changed));
  exec_up_rulebox.pack_start(exec_rulelabel, Gtk::PACK_SHRINK, 5);
  exec_up_rulebox.pack_start(exec_rulechooser, Gtk::PACK_SHRINK, 5);
  
  exec_lambdaminlabel.set_text("Lambda MIN");
  exec_lambdamaxlabel.set_text("Lambda MAX");
  exec_lambda_min.set_value(0.5);
  exec_lambda_max.set_value(1.0);
  exec_lambda_min.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::set_changed));
  exec_lambda_max.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::set_changed));
  exec_up_lambdabox.pack_start(exec_lambdaminlabel, Gtk::PACK_SHRINK, 5);
  exec_up_lambdabox.pack_start(exec_lambda_min, Gtk::PACK_SHRINK, 5);
  exec_up_lambdabox.pack_start(exec_lambdamaxlabel, Gtk::PACK_SHRINK, 5);
  exec_up_lambdabox.pack_start(exec_lambda_max, Gtk::PACK_SHRINK, 5);
  
  exec_vbox.pack_start(exec_up_hbox, Gtk::PACK_SHRINK, 5);

  result_scroll_III.add(result_box_III);
  result_scroll_3.add(result_box_3);
  rankacc_label.set_text("Rank acceptability indices");
  rankacc_label_3.set_text("Rank acceptability indices");
  smaaIII_rankacc_table.set_corner_name("Rank");
  smaa3_rankacc_table.set_corner_name("Rank");

  result_box_3.pack_start(rankacc_label_3, Gtk::PACK_SHRINK, 5);
  result_box_3.pack_start(smaa3_rankacc_table, Gtk::PACK_SHRINK, 5);

  result_box_III.pack_start(rankacc_label, Gtk::PACK_SHRINK, 5);
  result_box_III.pack_start(smaaIII_rankacc_table, Gtk::PACK_SHRINK, 5);
  pwwin_label.set_text("Pair-wise winning indices");
  result_box_III.pack_start(pwwin_label, Gtk::PACK_SHRINK, 5);
  result_box_III.pack_start(smaaIII_pwwin_table, Gtk::PACK_SHRINK, 5);
  incomp_label.set_text("Incomparability indices");
  result_box_III.pack_start(incomp_label, Gtk::PACK_SHRINK, 5);
  result_box_III.pack_start(smaaIII_incomp_table, Gtk::PACK_SHRINK, 5);

  catacc_label.set_text("Category acceptability indices");
  result_box_TRI.pack_start(catacc_label, Gtk::PACK_SHRINK, 5);
  result_box_TRI.pack_start(trires_table, Gtk::PACK_SHRINK, 5);
  result_scroll_TRI.add(result_box_TRI);

  tab_execute.add(exec_vbox);

  // add all the first tab
  tabs.append_page(tab_selectmethod, "Select method");

  // the buttons on bottom
  back_button.set_label(Glib::ustring("<- Go back"));
  forw_button.set_label(Glib::ustring("Continue ->"));
  vbox.pack_start(box_downbuttons, Gtk::PACK_SHRINK, 5, 5);
  box_downbuttons.pack_start(back_button, Gtk::PACK_SHRINK, 5, 5);
  box_downbuttons.pack_end(forw_button, Gtk::PACK_SHRINK, 5, 5);

  // connect signal handlers
  switch_page_conn = tabs.signal_switch_page().connect(sigc::mem_fun(*this, &MainWindow::on_switch_page));
  back_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_back_button_clicked));
  forw_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_forw_button_clicked));

  sm_rb_smaa2.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::activate_method_smaa2));
  sm_rb_smaaTRI.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::activate_method_smaaTRI));
  sm_rb_smaaIII.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::activate_method_smaaIII));
  sm_rb_smaa3.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::activate_method_smaa3));

  
  set_title(APPNAME);
  show_all_children();
  // disable button - smaa2 not supported yet
  sm_rb_smaa2.set_sensitive(false);
  sm_rb_smaaTRI.set_sensitive(true);
  sm_rb_smaaIII.set_sensitive(true);
  sm_rb_smaa3.set_sensitive(true);
  model_changed = false;

#ifdef DEBUG
  printf ("Gui initialized!\n");
#endif

  Gtk::MessageDialog dialog (*this, "Welcome to CSMAA v1.0 - (c) 2007 Tommi Tervonen", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_CLOSE, true);

#ifdef DEMO
  dialog.set_secondary_text("This is a DEMO Version - Unauthorized use prohibited, and will be punished with laws applicable in the country.\n\nFor information on buying the full version, contact the author by email: tommi.tervonen@it.utu.fi");
#else
  dialog.set_secondary_text("This is a FULL Version - Copying and unauthorized use prohibited, and will be punished with laws applicable in the country.");
#endif
  dialog.run();

}

void MainWindow::on_menu_about()
{
  Gtk::MessageDialog dialog (*this, "CSMAA v1.0 - (c) 2007 Tommi Tervonen", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_CLOSE, true);
  dialog.set_secondary_text("Unauthorized use prohibited, and will be punished with laws applicable in the country. This software is delivered \"as is\", that is, no responsibility is taken for whatever losses or damages the usage of this software might cause.\n\nFor buying the software, contact the author by email: tommi.tervonen@it.utu.fi");

  dialog.run();
}

int MainWindow::get_no_alts() const
{
  return no_alts;
}
int MainWindow::get_no_crit() const
{
  return no_crit;
}
int MainWindow::get_no_cats() const
{
  return no_cats;
}

MainWindow::~MainWindow()
{
  switch_page_conn.disconnect();

  DEL_VECTOR_CONTENTS(v_entry_altsnames);
  DEL_VECTOR_CONTENTS(v_entry_catsnames);
}

void MainWindow::on_forw_button_clicked()
{
  tabs.next_page();
}

void MainWindow::on_switch_page(GtkNotebookPage *page, guint i)
{ 
  if (i != 0)
    back_button.show();
  else
    back_button.hide();

  if (method == SMAA2)
    {
      switch(old_page)
	{
	case 1: // selectnos
	  //	  update_nos();
	  break;
	case 2: // input altnames
	  update_altnames();
	  break;
	case 3: // input criteria
	  update_criteriainfo();
	  break;
	}
    }
  else if (method == SMAATRI)
    {
      if (i == 10)
	forw_button.hide();
      else
	forw_button.show();

      switch(old_page)
	{
	case 1: // selectnos
	  //update_nos();
	  break;
	case 2: // input altnames
	  update_altnames();
	  break;
	case 3: // input catnames
	  update_catnames();
	  break;
	case 4: // input criteria
	  update_criteriainfo();
	  break;	  
	case 5: // input measurements
	  if (im_table.has_changed())
	    model_changed = true;
	  break;
	case 6: // input uncertainties
	  if (iuc_table.has_changed())
	    model_changed = true;
	  break;
	case 7: // input profiles
	  if (iprof_table.has_changed())
	    model_changed = true;
	  break;
	case 8: // input profile uncertainties
	  if (ipuc_table.has_changed())
	    model_changed = true;
	  break;
	case 9: // input preferences
	  if (ip_ord_table.has_changed())
	    model_changed = true;
	  if (ip_card_table.has_changed())
	    model_changed = true;
	  if (ip_exact_table.has_changed())
	    model_changed = true;
	  break;
	case 10: // execute
	  break;
	}
    }
  else if (method == SMAAIII)
    {
      if (i == 7)
	forw_button.hide();
      else
	forw_button.show();

      switch(old_page)
	{
	case 1: // selectnos
	  //update_nos();
	  break;
	case 2: // input altnames
	  update_altnames();
	  break;
	case 3: // input criteria
	  update_criteriainfo();
	  break;
	case 4: // input measurements
	  if (im_table.has_changed())
	    model_changed = true;
	  break;
	case 5: // input uncertainties
	  if (iuc_table.has_changed())
	    model_changed = true;
	  break;
	case 6: // input preferences
	  if (ip_ord_table.has_changed())
	    model_changed = true;
	  if (ip_card_table.has_changed())
	    model_changed = true;
	  if (ip_exact_table.has_changed())
	    model_changed = true;
	  break;
	case 7: // execute
	  break;
	}
    }
  else {
    back_button.hide();
    forw_button.hide();
  }

  if (model_changed && old_page != (int) i)
    {
      set_title(string(APPNAME) + " " + filename + " * ");
    }
  
  old_page = i; 

  queue_draw();
}

void MainWindow::update_altnames()
{
  if (!altname_changed) return;

  for (int i=0;i<get_no_alts();i++)
    {
      Glib::ustring s = v_entry_altsnames[i]->get_text();
      trires_table.set_alternative_name(i, s);
      smaaIII_rankacc_table.set_alternative_name(i, s);
      smaa3_rankacc_table.set_alternative_name(i, s);
      smaaIII_pwwin_table.set_alternative_name(i, s);
      smaaIII_pwwin_table.set_category_name(i, s);
      smaaIII_incomp_table.set_alternative_name(i, s);
      smaaIII_incomp_table.set_category_name(i, s);
      im_table.set_alternative_name(i, s);
      iuc_table.set_alternative_name(i, s);
    }

  altname_changed = false;
}

void MainWindow::update_catnames()
{
  if (!catname_changed) return;

  for (int i=0;i<get_no_cats();i++)
    {
      Glib::ustring s = v_entry_catsnames[i]->get_text();
      trires_table.set_category_name(i, s);
    }
  catname_changed = false;
}

void MainWindow::on_back_button_clicked()
{
  tabs.prev_page();
}

void MainWindow::activate_method_smaa2()
{
  method = SMAA2;
  label_no_cats.hide();
  cb_selectcats.hide();
  label_no_cats.set_child_visible(false);
  cb_selectcats.set_child_visible(false);
  input_c_table.disable_thresholds();

  while (tabs.get_n_pages() > 1)
    tabs.remove_page(tabs.get_n_pages()-1);

  tabs.append_page(tab_selectnos, "Select #");
  tabs.append_page(tab_inputaltnames, "Alternative names");
  tabs.append_page(tab_inputcriteria, "Criteria");
  tabs.append_page(tab_inputmeasurements, "Measurements");
  tabs.append_page(tab_inputuncertainties, "Uncertainties");
  tabs.append_page(tab_inputpreferences, "Preferences");

  input_c_table.set_only_cardinal(false);

  back_button.hide();
}

void MainWindow::activate_method_smaaTRI()
{
  method = SMAATRI;
  label_no_cats.show();
  cb_selectcats.show();
  label_no_cats.set_child_visible(true);
  cb_selectcats.set_child_visible(true);
  input_c_table.enable_thresholds();
  
  if (exec_up_rulebox.get_parent() == 0)
    exec_vbox.pack_start(exec_up_rulebox, Gtk::PACK_SHRINK, 5);
  if (exec_up_lambdabox.get_parent() == 0)
    exec_vbox.pack_start(exec_up_lambdabox, Gtk::PACK_SHRINK, 5);

  if (result_scroll_III.get_parent())
    exec_vbox.remove(result_scroll_III);
  if (result_scroll_3.get_parent())
    exec_vbox.remove(result_scroll_3);
  exec_vbox.pack_start(result_scroll_TRI, Gtk::PACK_EXPAND_WIDGET, 0);

  while (tabs.get_n_pages() > 1)
    tabs.remove_page(tabs.get_n_pages()-1);

  tabs.append_page(tab_selectnos, "Select #");
  tabs.append_page(tab_inputaltnames, "Alternative names");
  tabs.append_page(tab_inputcatnames, "Category names");
  tabs.append_page(tab_inputcriteria, "Criteria");
  tabs.append_page(tab_inputmeasurements, "Measurements");
  tabs.append_page(tab_inputuncertainties, "Uncertainties");
  tabs.append_page(tab_inputprofiles, "Prof meas");
  tabs.append_page(tab_inputprofuncertainties, "Prof uncert");
  tabs.append_page(tab_inputpreferences, "Preferences");
  tabs.append_page(tab_execute, "Execute");

  for (int i=0;i<get_no_crit();i++)
    if (input_c_table.get_type(i) == TYPE_ORD)
      {
	iuc_table.set_criterion_type(i, TYPE_UNIF_REAL);
	im_table.set_criterion_type(i, TYPE_UNIF_REAL);
	iprof_table.set_criterion_type(i, TYPE_UNIF_REAL);
	ipuc_table.set_criterion_type(i, TYPE_UNIF_REAL);
      }
	
  input_c_table.set_only_cardinal(true);

  tabs.show_all_children();
  back_button.hide();

  tabs.set_current_page(1);
  tabs.set_current_page(2);
  tabs.set_current_page(3);
  tabs.set_current_page(4);
  tabs.set_current_page(5);
  tabs.set_current_page(6);
  tabs.set_current_page(7);
  tabs.set_current_page(8);
  tabs.set_current_page(9);
  tabs.set_current_page(10);
  tabs.set_current_page(0);
}

void MainWindow::activate_method_smaaIII()
{
  method = SMAAIII;
  input_c_table.enable_thresholds();
  label_no_cats.set_child_visible(false);
  cb_selectcats.set_child_visible(false);
  label_no_cats.hide();
  cb_selectcats.hide();

  if (exec_up_rulebox.get_parent())
    exec_vbox.remove(exec_up_rulebox);
  if (exec_up_lambdabox.get_parent())
    exec_vbox.remove(exec_up_lambdabox);

  if (result_scroll_TRI.get_parent())
    exec_vbox.remove(result_scroll_TRI);
  if (result_scroll_3.get_parent())
    exec_vbox.remove(result_scroll_3);
  exec_vbox.pack_start(result_scroll_III, Gtk::PACK_EXPAND_WIDGET, 0);

  while (tabs.get_n_pages() > 1)
    tabs.remove_page(tabs.get_n_pages()-1);

  tabs.append_page(tab_selectnos, "Select #");
  tabs.append_page(tab_inputaltnames, "Alternative names");
  tabs.append_page(tab_inputcriteria, "Criteria");
  tabs.append_page(tab_inputmeasurements, "Measurements");
  tabs.append_page(tab_inputuncertainties, "Uncertainties");
  tabs.append_page(tab_inputpreferences, "Preferences");
  tabs.append_page(tab_execute, "Execute");

  input_c_table.set_only_cardinal(false);

  tabs.show_all_children();
  back_button.hide();

  tabs.set_current_page(1);
  tabs.set_current_page(2);
  tabs.set_current_page(3);
  tabs.set_current_page(4);
  tabs.set_current_page(5);
  tabs.set_current_page(6);
  tabs.set_current_page(7);
  tabs.set_current_page(0);
}

void MainWindow::activate_method_smaa3()
{
  method = SMAA3;
  input_c_table.enable_thresholds();
  label_no_cats.set_child_visible(false);
  cb_selectcats.set_child_visible(false);
  label_no_cats.hide();
  cb_selectcats.hide();

  if (exec_up_rulebox.get_parent())
    exec_vbox.remove(exec_up_rulebox);
  if (exec_up_lambdabox.get_parent())
    exec_vbox.remove(exec_up_lambdabox);

  if (result_scroll_TRI.get_parent())
    exec_vbox.remove(result_scroll_TRI);
  if (result_scroll_III.get_parent())
    exec_vbox.remove(result_scroll_III);
  exec_vbox.pack_start(result_scroll_3, Gtk::PACK_EXPAND_WIDGET, 0);

  while (tabs.get_n_pages() > 1)
    tabs.remove_page(tabs.get_n_pages()-1);

  tabs.append_page(tab_selectnos, "Select #");
  tabs.append_page(tab_inputaltnames, "Alternative names");
  tabs.append_page(tab_inputcriteria, "Criteria");
  tabs.append_page(tab_inputmeasurements, "Measurements");
  tabs.append_page(tab_inputuncertainties, "Uncertainties");
  tabs.append_page(tab_inputpreferences, "Preferences");
  tabs.append_page(tab_execute, "Execute");

  input_c_table.set_only_cardinal(false);

  tabs.show_all_children();
  back_button.hide();

  tabs.set_current_page(1);
  tabs.set_current_page(2);
  tabs.set_current_page(3);
  tabs.set_current_page(4);
  tabs.set_current_page(5);
  tabs.set_current_page(6);
  tabs.set_current_page(7);
  tabs.set_current_page(0);
}

void MainWindow::update_nos()
{
  int noa = cb_selectalts.get_active_row_number() + 1;
  int noc = cb_selectcrit.get_active_row_number() + 1;
  int noca = cb_selectcats.get_active_row_number() + 2;

  assert (noa > 0);
  assert (noc > 0);
  assert (noca > 0);

  if (noa != no_alts || noca != no_cats)
    {
      trires_table.resize(noa, noca);
    }
  
  if (noc != no_crit || noca != no_cats)
    {
      iprof_table.resize(noc, noca-1);
      ipuc_table.resize(noc, noca-1);
    }

  if (noa != no_alts || noc != no_crit)
    {
      im_table.resize(noc,noa);
      iuc_table.resize(noc,noa);
    }
  if (noa != no_alts) // update
    {
      smaaIII_rankacc_table.resize(noa, noa);
      smaa3_rankacc_table.resize(noa, noa);
      smaaIII_pwwin_table.resize(noa, noa);
      smaaIII_incomp_table.resize(noa, noa);
      input_c_table.set_no_alts(noa);
      model_changed = true;
      if (noa > no_alts)
	{
	  v_entry_altsnames.resize(noa);
	  
	  for (int i=no_alts;i<noa;i++)
	    {
	      char tempbuf[20];
	      sprintf(tempbuf, "Alternative %d", i+1);
	      v_entry_altsnames[i] = new Gtk::Entry();
	      v_entry_altsnames[i]->set_width_chars(ENTRYWIDTH);
	      v_entry_altsnames[i]->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_altname_changed));
	      v_entry_altsnames[i]->set_max_length(MAXNAMELENGTH);
	      v_entry_altsnames[i]->set_text(Glib::ustring(tempbuf));
	      tab_ian_vbox.pack_start(*v_entry_altsnames[i], Gtk::PACK_SHRINK, 5, 5);
	      v_entry_altsnames[i]->show();
	      
	      // add new components to the measurementstable
	      im_table.set_alternative_name(i, tempbuf);
	      iuc_table.set_alternative_name(i, tempbuf);
	      trires_table.set_alternative_name(i, tempbuf);
	      smaaIII_rankacc_table.set_alternative_name(i, tempbuf);
	      smaa3_rankacc_table.set_alternative_name(i, tempbuf);
	      smaaIII_pwwin_table.set_alternative_name(i, tempbuf);
	      smaaIII_pwwin_table.set_category_name(i, tempbuf);
	      smaaIII_incomp_table.set_alternative_name(i, tempbuf);
	      smaaIII_incomp_table.set_category_name(i, tempbuf);
	      sprintf (tempbuf, "%d", i+1);
	      smaaIII_rankacc_table.set_category_name(i, tempbuf);
	      smaa3_rankacc_table.set_category_name(i, tempbuf);
	    }
	}
      else // noa < no_alts
	{
	  for (int i=no_alts-1;i>=noa;i--)
	    {
	      tab_ian_vbox.remove(*v_entry_altsnames[i]);
	      delete v_entry_altsnames[i];
	    }
	  v_entry_altsnames.resize(noa);
	}
    }
  if (noc != no_crit) // update
    {
      model_changed = true;

      ip_card_table.resize(noc);
      ip_ord_table.resize(noc);
      ip_exact_table.resize(noc);
      input_c_table.resize(noc);

      if (noc > no_crit)
	{
	  for (int i=no_crit;i<noc;i++)
	    {
	      crittype t = input_c_table.get_type(i);
	      Glib::ustring n = input_c_table.get_name(i);
	      im_table.set_criterion_type(i, t);
	      iuc_table.set_criterion_type(i, t);
	      im_table.set_criterion_name(i, n);
	      iuc_table.set_criterion_name(i, n);
	      iprof_table.set_criterion_type(i, t);
	      iprof_table.set_criterion_name(i, n);
	      ipuc_table.set_criterion_type(i, t);
	      ipuc_table.set_criterion_name(i, n);
	      ip_card_table.set_criterion_name(i, n);
	      ip_ord_table.set_criterion_name(i, n);
	      ip_exact_table.set_criterion_name(i, n);

	    }
	}
      else // noc < no_crit
	{
	}
    }
  if (noca != no_cats) // update
    {
      model_changed = true;	  
      if (noca > no_cats)
	{
	  v_entry_catsnames.resize(noca);
	  
	  for (int i=no_cats;i<noca;i++)
	    {
	      char tempbuf[20];
	      sprintf(tempbuf, "Category %d", i+1);
	      v_entry_catsnames[i] = new Gtk::Entry();
	      v_entry_catsnames[i]->set_width_chars(ENTRYWIDTH);
	      v_entry_catsnames[i]->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::on_catname_changed));
	      v_entry_catsnames[i]->set_max_length(MAXNAMELENGTH);
	      v_entry_catsnames[i]->set_text(Glib::ustring(tempbuf));
	      tab_icatsn_vbox.pack_start(*v_entry_catsnames[i], Gtk::PACK_SHRINK, 5, 5);
	      v_entry_catsnames[i]->show();

	      trires_table.set_category_name(i, tempbuf);

	      if (i > 0)
		{
		  sprintf(tempbuf, "Profile %d", i);
		  iprof_table.set_alternative_name(i-1, tempbuf);
		  ipuc_table.set_alternative_name(i-1, tempbuf);
		}
	    }
	}
      else // noca < no_cats
	{
	  for (int i=no_cats-1;i>=noca;i--)
	    {
	      tab_icatsn_vbox.remove(*v_entry_catsnames[i]);
	      if (v_entry_catsnames[i])
		delete v_entry_catsnames[i];
	    }
	  v_entry_catsnames.resize(noca);
	}
    }
  no_alts = noa;
  no_crit = noc;
  no_cats = noca;
}
  
void MainWindow::update_criteriainfo()
{
  if (input_c_table.has_changed())
    {
      for (int i=0;i<get_no_crit();i++)
	{
	  Glib::ustring n = input_c_table.get_name(i);
	  ip_ord_table.set_criterion_name(i, n);
	  ip_card_table.set_criterion_name(i, n);
	  ip_exact_table.set_criterion_name(i, n);

	  im_table.set_criterion_name(i, n);
	  iuc_table.set_criterion_name(i, n);
	  iprof_table.set_criterion_name(i, n);
	  ipuc_table.set_criterion_name(i, n);

	  crittype t = input_c_table.get_type(i);
	  im_table.set_criterion_type(i, t);
      	  iuc_table.set_criterion_type(i, t);
	  iprof_table.set_criterion_type(i, t);
	  ipuc_table.set_criterion_type(i, t);

	  input_c_table.clear_changed();
	  model_changed = true;
	}
    }
}

void MainWindow::on_altname_changed()
{
  model_changed = true;
  altname_changed = true;
}

void MainWindow::on_catname_changed()
{
  model_changed = true;
  catname_changed = true;
}

void MainWindow::on_ip_card_button_clicked()
{
  if (input_cardpref_on)
	{
	  ip_table.remove(ip_card_table);
	  ip_card_button.set_label("Add cardinal (upper and lower bound) preferences");
	  input_cardpref_on = false;
	  ip_ord_button.set_sensitive(true);
	  ip_ex_button.set_sensitive(true);
	}
  else
	{
	  ip_table.attach(ip_card_table, 0, 3, 1, 2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
	  input_cardpref_on = true;
	  ip_card_button.set_label("Remove cardinal preferences");
	  ip_ord_button.set_sensitive(false);
	  ip_ex_button.set_sensitive(false);
	  ip_card_table.show();
	}
  model_changed = true;
}


void MainWindow::on_ip_ex_button_clicked()
{
  if (input_expref_on)
	{
	  ip_table.remove(ip_exact_table);
	  ip_ex_button.set_label("Add exact preferences");
	  input_expref_on = false;
	  ip_ord_button.set_sensitive(true);
	  ip_card_button.set_sensitive(true);
	}
  else
	{
	  ip_table.attach(ip_exact_table, 0, 3, 1, 2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
	  input_expref_on = true;
	  ip_ex_button.set_label("Remove exact preferences");
	  ip_ord_button.set_sensitive(false);
	  ip_card_button.set_sensitive(false);
	  ip_exact_table.show();
	}
  model_changed = true;
}

void MainWindow::on_ip_ord_button_clicked()
{
  if (input_ordpref_on)
	{
	  ip_table.remove(ip_ord_table);
	  input_ordpref_on = false;
	  ip_ord_button.set_label("Add ordinal (ranking) preferences");
	  ip_card_button.set_sensitive(true);
	  ip_ex_button.set_sensitive(true);
	}
  else
	{
	  ip_table.attach(ip_ord_table, 0, 3, 1, 2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
	  input_ordpref_on = true;
	  ip_ord_button.set_label("Remove ordinal preferences");
	  ip_card_button.set_sensitive(false);
	  ip_ex_button.set_sensitive(false);
	  ip_ord_table.show();
	}
  model_changed = true;
}

void MainWindow::on_but_execute_clicked()
{
  if (run) // cancel the running
    {
      run = false;
      th->join();
    }
  else // start executing
    {
      sensitivize_tabs(false);
      exec_progbar.set_text("Processing...");
      but_execute.set_label(Glib::ustring("Cancel"));
      exec_up_box.set_sensitive(false);
      exec_up_rulebox.set_sensitive(false);
      exec_lambda_min.set_sensitive(false);
      exec_lambda_max.set_sensitive(false);
      run = true;
      th = Glib::Thread::create(sigc::mem_fun(*this, &MainWindow::simulation_run), true);
    }
}

void MainWindow::on_menu_save()
{
  if (filename.size() == 0)
    {
      on_menu_save_as();
      return;
    }
  try{
    save_model(filename);
  }
  catch (std::string &s)
    {
      info_dialog(std::string("Error saving file: ") + s);
    }
}

void MainWindow::on_menu_save_as()
{
  Gtk::FileChooserDialog dialog("Choose a file to save as", Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog.set_transient_for(*this);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  Gtk::FileFilter filter_bin;
  filter_bin.set_name("CSMAA Files");
  filter_bin.add_pattern("*.csm");
  dialog.add_filter(filter_bin);

  //  Gtk::FileFilter filter_all;
  //filter_all.set_name("Any files");
  //filter_all.add_pattern ("*");
  //dialog.add_filter(filter_all);


  bool cont = true;
  while (cont)
    {
      int result = dialog.run();
      
      std::string fname;
      
      switch (result)
	{
	case Gtk::RESPONSE_OK:
	  fname = dialog.get_filename();
	  if (fname.find(".csm", 0) == fname.npos)
	    fname = fname + ".csm";
	  
	  try{
	    save_model(fname);
	    cont = false;
	  }
	  catch (std::string &s)
	    {
	      info_dialog(std::string("Error saving file: ") + s);
	    }
	  break;
	case Gtk::RESPONSE_CANCEL:
	  cont= false;
	  break;
	default:
	  break;
	}
    }
}

void MainWindow::on_menu_open()
{
  Gtk::FileChooserDialog dialog("Choose a file to open", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  Gtk::FileFilter filter_bin;
  filter_bin.set_name("CSMAA Files");
  filter_bin.add_pattern("*.csm");
  dialog.add_filter(filter_bin);

  Gtk::FileFilter filter_all;
  filter_all.set_name("Any files");
  filter_all.add_pattern ("*");
  dialog.add_filter(filter_all);

  int result = dialog.run();
  
  std::string fname;
  switch (result)
    {
    case Gtk::RESPONSE_OK:
      fname = dialog.get_filename();
      dialog.hide();

      try{
	load_model(fname);
	tabs.set_current_page(tabs.get_n_pages()-1);
      }
      catch (std::string &s)
	{
	  info_dialog(std::string("Error opening model file: ") + s);
	}
      break;
    case Gtk::RESPONSE_CANCEL:
      break;
    default:
      break;
    }
}

void MainWindow::on_menu_quit()
{
  on_switch_page(NULL, tabs.get_current_page());
  if (run)
    {
      run = false;
      th->join();
    }
  if (model_changed)
    {
      Gtk::MessageDialog dialog (*this, "Model changed", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO, true);
      dialog.set_secondary_text("Model has changed since last save. Do you want to exit anyway?");
      int res = dialog.run();
      if (res == Gtk::RESPONSE_YES)
	hide();
    }
  else
    hide();
}

void MainWindow::save_model(const string & fname) throw (string)
{
  ofstream f;
  // save stuff on this page
  on_switch_page(NULL, tabs.get_current_page());
  f.open(fname.c_str(), ios::out | ios::trunc);
  if (!f.is_open())
    throw std::string ("Error opening file for writing");

  f << method << std::endl;

  f << get_no_alts() << std::endl;
  f << get_no_crit() << std::endl;
  f << get_no_cats() << std::endl;
  
  for (int i=0;i<get_no_alts();i++)
    f << v_entry_altsnames[i]->get_text().raw() << std::endl;
  for (int i=0;i<get_no_cats();i++)
    f << v_entry_catsnames[i]->get_text().raw() << std::endl;
  //  for (int i=0;i<get_no_cats()-1;i++)
  //  f << prof_names[i]->get_text().raw();
  f << input_c_table;
  f << im_table;
  f << iuc_table;
  f << iprof_table;
  f << ipuc_table;
  f << input_cardpref_on << std::endl;
  f << ip_card_table;
  f << input_ordpref_on << std::endl;
  f << ip_ord_table;
  f << input_expref_on << std::endl;
  f << ip_exact_table;
  f << exec_up_box.get_active_text().raw() << std::endl;
  bool optimistic = (exec_rulechooser.get_active_row_number() == 1);
  f << optimistic << std::endl;
  f << exec_lambda_min.get_value() << std::endl;
  f << exec_lambda_max.get_value() << std::endl;


  if (f.bad())
    throw string ("write error");
  else
    {
      clear_changed_info();
      filename = fname;
      set_title(string(APPNAME) + " " + filename);
    }
}

void MainWindow::clear_changed_info()
{
  // clear changed flags
  model_changed = false;
  im_table.clear_changed();
  iuc_table.clear_changed();
  input_c_table.clear_changed();
  iprof_table.clear_changed();
  ipuc_table.clear_changed();
  ip_ord_table.clear_changed();
  ip_card_table.clear_changed();
  ip_exact_table.clear_changed();
}

void MainWindow::load_model(const string & fname) throw (string)
{
  ifstream f;
  int _method, _no_alts, _no_crit, _no_cats;
  bool tmpb;
  char buf[256];
  f.open(fname.c_str(), ios::in);
  if (!f.is_open())
    throw std::string ("error opening file for reading");

  f >> _method;
  f >> _no_alts;
  f >> _no_crit;
  f >> _no_cats;


#ifdef DEBUG
  printf ("File read with %d alts, %d crit, %d cats\n", _no_alts, _no_crit, _no_cats);
  printf ("Method: %d\n", _method);
#endif

  switch (_method)
    {
    case SMAA2:
      sm_rb_smaa2.clicked();
      break;
    case SMAATRI:
      sm_rb_smaaTRI.clicked();
      break;
    case SMAAIII:
      sm_rb_smaaIII.clicked();
      break;
    case SMAA3:
      sm_rb_smaa3.clicked();
      break;
    default:
      throw string ("unknown method");
    }

  cb_selectalts.set_active(_no_alts-1);
  cb_selectcrit.set_active(_no_crit-1);
  cb_selectcats.set_active(_no_cats-2);

  update_nos();

  for (int i=0;i<get_no_alts();i++)
    {
      ws(f);
      f.getline(buf, 256);
      v_entry_altsnames[i]->set_text(Glib::ustring(buf));
#ifdef DEBUG
      printf ("Alternative %i name %s\n", i+1, buf);
#endif
    }
  update_altnames();
  for (int i=0;i<get_no_cats();i++)
    {
      ws(f);
      f.getline(buf, 256);
      v_entry_catsnames[i]->set_text(Glib::ustring(buf));
#ifdef DEBUG
      printf ("Category %i name %s\n", i+1, buf);
#endif
      trires_table.set_category_name(i, buf);
    }
  update_catnames();

  f >> input_c_table;
#ifdef DEBUG
  std::cout << input_c_table;
#endif
  f >> im_table;
#ifdef DEBUG
  std::cout << im_table;
#endif
  f >> iuc_table;
#ifdef DEBUG
  std::cout << iuc_table;
#endif
  f >> iprof_table;
#ifdef DEBUG
  std::cout << iprof_table;
#endif
  f >> ipuc_table;
#ifdef DEBUG
  std::cout << ipuc_table;
#endif
  f >> tmpb;
#ifdef DEBUG
  std::cout << "Cardinal preferences enabled: " << (tmpb ? "yes" : "no") << std::endl;
#endif
  f >> ip_card_table;
#ifdef DEBUG
  std::cout << ip_card_table;
#endif
  if (tmpb && !input_cardpref_on)
    ip_card_button.clicked();
  else if (!tmpb && input_cardpref_on)
    ip_card_button.clicked();
  f >> tmpb;
#ifdef DEBUG
  std::cout << "Ordinal preferences enabled: " << (tmpb ? "yes" : "no") << std::endl;
#endif
  f >> ip_ord_table;
  if (tmpb && !input_ordpref_on)
    ip_ord_button.clicked();
  else if (!tmpb && input_ordpref_on)
    ip_ord_button.clicked();

  f >> tmpb;
#ifdef DEBUG
  std::cout << "Exact preferences enabled: " << (tmpb ? "yes" : "no") << std::endl;
#endif
  f >> ip_exact_table;
  if (tmpb && !input_expref_on)
    ip_ex_button.clicked();
  else if (!tmpb && input_expref_on)
    ip_ex_button.clicked();

  ws(f);
  f.getline (buf, 256);
  exec_up_box.set_active_text(Glib::ustring(buf));
  bool optimistic;
  f >> optimistic;
  if (optimistic)
    exec_rulechooser.set_active(1);
  else
    exec_rulechooser.set_active(0);
  double lmin, lmax;
  f >> lmin;
  f >> lmax;
  exec_lambda_min.set_value(lmin);
  exec_lambda_max.set_value(lmax);

  if (f.bad())
    throw string ("read error");
  else
    {
      clear_changed_info();
      filename = fname;
      set_title(string(APPNAME) + " " + filename);
    }
}


void MainWindow::info_dialog(std::string text)
{
  Gtk::MessageDialog dialog (*this, "Message");
  dialog.set_secondary_text(text);
  dialog.run();
}

void MainWindow::sensitivize_tabs(bool sen)
{
  tab_selectmethod.set_sensitive(sen);
  tab_selectnos.set_sensitive(sen);
  tab_inputcatnames.set_sensitive(sen);
  tab_inputcriteria.set_sensitive(sen);
  tab_inputprofiles.set_sensitive(sen);
  tab_inputaltnames.set_sensitive(sen);
  tab_inputmeasurements.set_sensitive(sen);
  tab_inputuncertainties.set_sensitive(sen);
  tab_inputpreferences.set_sensitive(sen);
  tab_inputprofuncertainties.set_sensitive(sen);
  tab_inputprofiles.set_sensitive(sen);
}

void MainWindow::simulation_run()
{
  electre * ele = 0;
  electre_tri *tri = 0;
  electre_base *ebase = 0;
  int no_crit = get_no_crit();
  int no_alts = get_no_alts();
  int no_cats = get_no_cats();
  string st_iter = exec_up_box.get_active_text().raw();
  iters = atoi(st_iter.c_str());
  double *weights = new double[no_crit];
  double *tmpw = new double[no_crit];
  std::vector<int> ranks = ip_ord_table.get_criterion_ranks();
  double sum_lowerbounds = 0.0;
  std::string str("x");
  int *rank_acc_hits = 0, *pw_win_hits = 0, *incomp_hits = 0;
  int *catacc_hits = 0;
  double lmin = exec_lambda_min.get_value();
  double lmax = exec_lambda_max.get_value();
  curiter = 0;
  vector<bool> has_upperb(no_crit);
  vector<double> upperb(no_crit);
  vector<double> lowerb(no_crit);
  vector<crittype> types(no_crit);
  vector<int> card_ths(no_crit);
  vector<double> indif_th_b(no_crit);
  vector<double> pref_th_b(no_crit);
  vector<double> veto_th_b(no_crit);
  vector<double> indif_th_l(no_crit);
  vector<double> pref_th_l(no_crit);
  vector<double> veto_th_l(no_crit);

  double * uncerts = new double[no_alts * no_crit];
  double * meas = new double[no_alts * no_crit];
  double *puncerts = 0;
  double *pmeas = 0;

  for (int i=0;i<no_crit;i++)
    {
      has_upperb[i] = ip_card_table.has_upper_bound(i);

      if (has_upperb[i])
	upperb[i] = ip_card_table.get_upper_bound(i);
      if (ip_card_table.has_lower_bound(i))
	lowerb[i] = ip_card_table.get_lower_bound(i);
      else
	lowerb[i] = 0.0;

      types[i] = input_c_table.get_type(i);
      card_ths[i] = input_c_table.get_card_threshold_type(i);
      
      if (card_ths[i] == ThresholdBox::UNIF || card_ths[i] == ThresholdBox::UNIFPROS)
	{
	  indif_th_b[i] = input_c_table.get_indif_threshold_base(i);
	  pref_th_b[i] = input_c_table.get_pref_threshold_base(i);

	  if (input_c_table.has_veto(i))
	    {
	      veto_th_b[i] = input_c_table.get_veto_threshold_base(i);
	      veto_th_l[i] = input_c_table.get_veto_threshold_limit(i);
	    }
	      
	  indif_th_l[i] = input_c_table.get_indif_threshold_limit(i);
	  pref_th_l[i] = input_c_table.get_pref_threshold_limit(i);

	  
	}
    }

  for (int k=0;k<no_alts;k++)
    for (int j=0;j<no_crit;j++)
      uncerts[k*no_crit+j] = iuc_table.get_measurement(k, j);

  for (int k=0;k<no_alts;k++)
    for (int j=0;j<no_crit;j++)
      meas[k*no_crit+j] = im_table.get_measurement(k, j);

  srand(666);

  if (input_expref_on)
    for (int i=0;i<no_crit;i++)
      weights[i] = ip_exact_table.get_weight(i);
  
  if (method == SMAAIII || method == SMAA3)
    {

      rank_acc_hits = new int [no_alts*no_alts];
      fill (rank_acc_hits, rank_acc_hits+(no_alts*no_alts), 0);
      
      pw_win_hits = new int [no_alts*no_alts];
      fill (pw_win_hits, pw_win_hits+(no_alts*no_alts), 0);
      
      incomp_hits = new int [no_alts*no_alts];
      fill (incomp_hits, incomp_hits+(no_alts*no_alts), 0);

      // create the model
      ele = new electre(no_alts, no_crit, -0.15, 0.3);
      
      for (int i=0;i<no_crit;i++)
	{
	  electre_criterion *c;
	  if (input_c_table.get_type(i) == TYPE_ORD) //ordinal criterion
	    {
	      c = new electre_criterion(str, str, weights[i], false, 0.0, 0.0, 0.0, 1.0);
	    }
	  else
	    {
	      double pth = input_c_table.get_pref_threshold_base(i);
	      double idth = input_c_table.get_indif_threshold_base(i);

	      if (input_c_table.is_threshold_percentage(i))
		{
		  if(input_c_table.has_veto(i))
		    {
		      c = new electre_criterion(str, str, weights[i], input_c_table.is_ascending(i), idth / 100.0, 0.0, pth / 100.0, 0.0, input_c_table.get_veto_threshold_base(i) / 100.0, 0.0);
		    }
		  else
		    c = new electre_criterion(str, str, weights[i], input_c_table.is_ascending(i), idth / 100.0, 0.0, pth / 100.0, 0.0);
		}
	      else
		{
		  if(input_c_table.has_veto(i))
		    {
		      c = new electre_criterion(str, str, 1.0, input_c_table.is_ascending(i), 0.0, idth, 0.0, pth, 0.0, input_c_table.get_veto_threshold_base(i));
		    }
		  else
		    c = new electre_criterion(str, str, 1.0, input_c_table.is_ascending(i), 0.0, idth, 0.0, pth);
		}
	    }
	  ele->set_criterion(i, c);
	}
      for (int i=0;i<no_alts;i++)
	{
	  double *cvals = new double[no_crit];
	  fill (cvals, cvals+no_crit, 0.0);
	  electre_alternative *al = new electre_alternative(str, str, no_crit, cvals);
	  ele->set_alternative(i, al);
	}
    }
  else if (method == SMAATRI)
    {
      puncerts = new double[(no_alts-1) * no_crit];
      pmeas = new double[(no_alts-1) * no_crit];

      for (int k=0;k<no_cats-1;k++)
	for (int j=0;j<no_crit;j++)
	  puncerts[k*no_crit+j] = ipuc_table.get_measurement(k, j);

      for (int k=0;k<no_cats-1;k++)
	for (int j=0;j<no_crit;j++)
	  pmeas[k*no_crit+j] = iprof_table.get_measurement(k, j);
      
      catacc_hits = new int[no_alts*no_cats];
      fill (catacc_hits, catacc_hits+(no_alts*no_cats), 0);
      
      // create the model
      tri = new electre_tri(no_cats, no_crit, no_alts);
      tri->set_rule(exec_rulechooser.get_active_row_number() == 1);
      
      for (int i=0;i<no_crit;i++)
	{
	  electre_criterion *c;
	  if (input_c_table.get_type(i) == TYPE_ORD) //ordinal criterion
	    {
	      c = new electre_criterion(str, str, weights[i], false, 0.0, 0.0, 0.0, 1.0);
	    }
	  else
	    {
	      double pth = input_c_table.get_pref_threshold_base(i);
	      double idth = input_c_table.get_indif_threshold_base(i);
	      
	      if (input_c_table.is_threshold_percentage(i))
		{
		  if(input_c_table.has_veto(i))
		    c = new electre_criterion(str, str, weights[i], input_c_table.is_ascending(i), idth / 100.0, 0.0, pth / 100.0, 0.0, input_c_table.get_veto_threshold_base(i) / 100.0, 0.0);
		  else
		    c = new electre_criterion(str, str, weights[i], input_c_table.is_ascending(i), idth / 100.0, 0.0, pth / 100.0, 0.0);
		}
	      else
		{
		  if(input_c_table.has_veto(i))
		    {
		      c = new electre_criterion(str, str, 1.0, input_c_table.is_ascending(i), 0.0, idth, 0.0, pth, 0.0, input_c_table.get_veto_threshold_base(i));
		    }
		  else
		    c = new electre_criterion(str, str, 1.0, input_c_table.is_ascending(i), 0.0, idth, 0.0, pth);
		}
	    }
	  tri->set_criterion(i, c);
	}
      for (int i=0;i<no_alts;i++)
	{
	  double *cvals = new double[no_crit];
	  fill (cvals, cvals+no_crit, 0.0);
	  electre_alternative *al = new electre_alternative(str, str, no_crit, cvals);
	  tri->set_alternative(i, al);
	}
      for (int i=0;i<no_cats-1;i++)
	{
	  double *cvals = new double[no_crit];
	  fill (cvals, cvals+no_crit, 0.0);
	  electre_alternative *al = new electre_alternative(str, str, no_crit, cvals);
	  tri->set_profile(i, al);
	}
    }
  if (method == SMAAIII || method == SMAA3)
    ebase = ele;
  else if (method == SMAATRI)
    ebase = tri;

  if (method == SMAA3)
    ele->set_rule_maximin();

  if (input_cardpref_on)
    {
      for (int i=0;i<no_crit;i++)
	if (ip_card_table.has_lower_bound(i))
	  sum_lowerbounds += ip_card_table.get_lower_bound(i);
    }

  // setup values for ordinal criteria
  for (int i=0;i<no_crit;i++)
    if (input_c_table.get_type(i) == TYPE_ORD)
      {
	for (int j=0;j<no_alts;j++)
	  ebase->get_alternative(j)->set_value(i, im_table.get_measurement(j, i));
      }
  // setup done, go for iterations
  for (curiter=0;curiter<iters && run;curiter++)
    {
      // create lambda cutting level for TRI
      if (method == SMAATRI)
	{
	  double lam = rand_interval (lmin, lmax);
	  tri->set_lambda(lam);
	}
      // sample criteria measurements
      for (int j=0;j<no_crit;j++)
	{
	  int typ = types[j];
	  if (typ == TYPE_ORD) // then we have already sampled!
	    {
	    }
	  else if (typ == TYPE_UNIF_REAL)
	    {
	      for (int k=0;k<no_alts;k++)
		{
		  double unc = uncerts[k*no_crit+j];
		  double val = meas[k*no_crit+j];
		  double m = rand_meanpm (val, unc);
		  ebase->get_alternative(k)->set_value(j, m);
		  if (method == SMAATRI && k < (no_cats-1))
		    {
		      double punc = puncerts[k*no_crit+j];
		      double pval = pmeas[k*no_crit+j];
		      double pm = rand_meanpm(pval, punc);
		      tri->get_profile(k)->set_value(j, pm);
		    }
		}
	    }
	  else if (typ == TYPE_UNIF_DISC)
	    {
	      for (int k=0;k<no_alts;k++)
		{
		  double unc = uncerts[k*no_crit+j];
		  double val = meas[k*no_crit+j];

		  double meanv = (val+unc)/2.0;
		  double diff = fabs(val-unc);

		  double m = rand_meanpm(meanv, diff);
		  m = round(m);

		  ebase->get_alternative(k)->set_value(j, m);
		  if (method == SMAATRI && k < (no_cats-1))
		    {
		      double punc = puncerts[k*no_crit+j];
		      double pval = pmeas[k*no_crit+j];

		      double pmeanv = (pval+punc)/2.0;
		      double pdiff = fabs(pval-punc);

		      double pm = rand_meanpm(pmeanv, pdiff);
		      pm = round(pm);
		      tri->get_profile(k)->set_value(j, pm);
		    }
		}
	    }
	  else if (typ == TYPE_GAUS)
	    {
	      for (int k=0;k<no_alts;k++)
		{
		  double unc = uncerts[k*no_crit+j];
		  double val = meas[k*no_crit+j];
		  double m = create_gaussian(val, unc);
		  ebase->get_alternative(k)->set_value(j, m);
		  if (method == SMAATRI && k < (no_cats-1))
		    {
		      double punc = puncerts[k*no_crit+j];
		      double pval = pmeas[k*no_crit+j];
		      double pm = create_gaussian(pval, punc);
		      tri->get_profile(k)->set_value(j, pm);
		    }
		}
	    }	  
	} // criterion measurements sampled

      // then create weights
      if (input_ordpref_on)
	{
	  create_sumtoone_sorted(tmpw, no_crit);
	  for (int j=0;j<no_crit;j++)
	    weights[j] = tmpw[no_crit-ranks[j]];
	}
      else if (input_cardpref_on)
	{
	  bool go = true;
	  while (go)
	    {
	      go = false;
	      create_sumto_rand(weights, no_crit, 1.0-sum_lowerbounds);
	      for (int j=0;j<no_crit && !go;j++)
		{
		  weights[j] += lowerb[j];
		  if (has_upperb[j])
		    if (weights[j] > upperb[j])
		      go = true;
		}
	    }
	} 
      else if (!input_expref_on)
	create_sumtoone_rand(weights, no_crit);
      // weights created

      if (method == SMAAIII || method == SMAATRI || method == SMAA3)
	ebase->set_weights(weights);

      // create thresholds
      for (int j=0;j<no_crit;j++)
	{
	  if (card_ths[j] == ThresholdBox::UNIF || card_ths[j] == ThresholdBox::UNIFPROS)
	    {
	      double in_th = rand_meanpm(indif_th_b[j], indif_th_l[j]);
	      double pr_th = rand_meanpm(pref_th_b[j], pref_th_l[j]);
	      double ve_th = 0.0;
	      if (input_c_table.has_veto(j))
		{
		  double ve_th = rand_meanpm(veto_th_b[j], veto_th_l[j]);
		}

	      if (card_ths[j] == ThresholdBox::UNIF)
		{
		  ebase->set_direct_pref_th(j, pr_th);
		  ebase->set_direct_indif_th(j, in_th);
		  if (input_c_table.has_veto(j))
		      ebase->set_direct_veto_th(j, ve_th);
		}
	      else // UNIFPROS
		{
		  ebase->set_pros_pref_th(j, pr_th / 100.0);
		  ebase->set_pros_indif_th(j, in_th / 100.0);
		  if (input_c_table.has_veto(j))
		    ebase->set_pros_veto_th(j, ve_th / 100.0);
		}
	    }
	}

      // execute
      if(method == SMAAIII || method == SMAATRI || method == SMAA3)
	ebase->analyze();

      if (method == SMAAIII)
	{
	  // update hitcounters - rank acceptabilities
	  for (int j=0;j<no_alts;j++)
	    rank_acc_hits[j*no_alts+ele->get_preorderrank(j)-1]++; // -1 as ranks are 1,2,...
	  
	  // pw winning indices & incomparability indices
	  for (int j=0;j<no_alts;j++)
	    for (int k=0;k<no_alts;k++)
	      {
		if (ele->get_preorderrank(j) < ele->get_preorderrank(k))
		  pw_win_hits[j*no_alts+k]++;
		if (ele->are_incomparable(j, k))
		  incomp_hits[j*no_alts+k]++;
	      }
	}
      else if (method == SMAA3)
	{
	  // update hitcounters - rank acceptabilities
	  for (int j=0;j<no_alts;j++)
	    rank_acc_hits[j*no_alts+ele->get_final_preorderrank(j)-1]++; // -1 as ranks are 1,2,...
	  
	}
      else if (method == SMAATRI)
	{
	  // update hitcounters - category acceptability indices
	  for (int j=0;j<no_alts;j++)
	    {
	      catacc_hits[(j*no_cats)+tri->get_class(j)]++;
	    }
	}

      // update runstatus
      if ((curiter+1) % 100 == 0)
	{
	  if (method == SMAAIII)
	    {
	      //update the indices
	      for (int j=0;j<no_alts;j++)
		for (int k=0;k<no_alts;k++)
		  {
		    smaaIII_rankacc_table.set_value(j, k, 100.0 * (rank_acc_hits[j*no_alts+k] / (double) (curiter+1)));
		    smaaIII_pwwin_table.set_value(j, k, 100.0 * (pw_win_hits[j*no_alts+k] / (double) (curiter+1)));
		    smaaIII_incomp_table.set_value(j, k, 100.0 * (incomp_hits[j*no_alts+k] / (double) (curiter+1)));
		  }
	    }
	  if (method == SMAA3)
	    {
	      //update the indices
	      for (int j=0;j<no_alts;j++)
		for (int k=0;k<no_alts;k++)
		  smaa3_rankacc_table.set_value(j, k, 100.0 * (rank_acc_hits[j*no_alts+k] / (double) (curiter+1)));
	    }
	  else if (method == SMAATRI)
	    {
	      // update cat acc indices
	      for (int j=0;j<no_alts;j++)
		for (int k=0;k<no_cats;k++)
		  {
		    trires_table.set_value(j, k, 100.0 * (catacc_hits[j*no_cats+k] / (double) (curiter+1)));
		  }

	    }
	  signal_update_runstatus();
	}
    } // iteration end
  if (!run) signal_update_runstatus();

  delete[] weights;
  delete[] tmpw;
  delete[] uncerts;
  delete[] meas;

  if (method == SMAAIII || method == SMAA3)
    {
      delete ele;
      delete[] pw_win_hits;
      delete[] rank_acc_hits;
      delete[] incomp_hits;
    }
  else if (method == SMAATRI)
    {
      delete tri;
      delete[] catacc_hits;
      delete[] puncerts;
      delete[] pmeas;
    }

}

void MainWindow::update_runstatus()
{
  if (curiter == iters)
    {
      sensitivize_tabs(true);
      but_execute.set_label(Glib::ustring("Re-execute"));
      but_execute.show();
      exec_progbar.set_fraction(1.0);
      exec_progbar.set_text("Done");
      exec_up_box.set_sensitive(true);
      exec_up_rulebox.set_sensitive(true);
      exec_lambda_max.set_sensitive(true);
      exec_lambda_min.set_sensitive(true);
      run = false;
    }
  else if (!run)
    {
      but_execute.set_label(Glib::ustring("Execute"));
      but_execute.show();
      //      exec_progbar.set_fraction(0.0);
      exec_progbar.set_text("Inactive");
      exec_up_box.set_sensitive(true);
      exec_up_rulebox.set_sensitive(true);
      exec_lambda_max.set_sensitive(true);
      exec_lambda_min.set_sensitive(true);
      sensitivize_tabs(true);
    }
  else
    {
      double fracdone = (double) curiter / (double) iters;
      exec_progbar.set_fraction(fracdone);
    }
}

void MainWindow::on_iuc_but_clicked(double am)
{
  for (int j=0;j<get_no_crit();j++)
    {
      if (input_c_table.get_type(j) == TYPE_ORD || input_c_table.get_type(j) == TYPE_UNIF_DISC)
	continue;
      
      for (int i=0;i<get_no_alts();i++)
	{
	  double val = im_table.get_measurement(i, j);
	  iuc_table.set_measurement(am * val, i, j);
	}
    }
}


void MainWindow::on_ipuc_but_clicked(double am)
{
  for (int j=0;j<get_no_crit();j++)
    {
      if (input_c_table.get_type(j) == TYPE_ORD || input_c_table.get_type(j) == TYPE_UNIF_DISC)
	continue;
      
      for (int i=0;i<get_no_cats()-1;i++)
	{
	  double val = iprof_table.get_measurement(i, j);
	  ipuc_table.set_measurement(am * val, i, j);
	}
    }
}
