libs= `pkg-config gtkmm-2.4 gthread-2.0 --libs` 
objs=mainwindow.o main.o inputcriteriontable.o numberentry.o rankcomboboxset.o classificationcell.o \
		measurementstable.o changetrackable.o cardinalpreferencestable.o ordinalpreferencestable.o \
		resultstable.o ../electre/electre.o thresholdbox.o simulation.o exactpreferencestable.o
all: csmaa

CXXFLAGS=-Wall -g -DDEBUG `pkg-config gtkmm-2.4 --cflags`
#CXXFLAGS=-Wall -O `pkg-config gtkmm-2.4 --cflags`

exactpreferencestable.o: exactpreferencestable.h exactpreferencestable.cpp numberentry.h changetrackable.h globals.h
thresholdbox.o: thresholdbox.h thresholdbox.cpp numberentry.h changetrackable.h
main.o: main.cpp
mainwindow.o: mainwindow.cpp mainwindow.h inputcriteriontable.h \
		measurementstable.h cardinalpreferencestable.h ordinalpreferencestable.h globals.h \
		resultstable.h ../electre/electre.h numberentry.h simulation.h exactpreferencestable.h
classificationcell.o: classificationcell.h classificationcell.cpp
resultstable.o: resultstable.h resultstable.cpp classificationcell.h globals.h
main.o: main.cpp
simulation.o: simulation.h simulation.cpp
numberentry.o: numberentry.h numberentry.cpp
inputcriteriontable.o: inputcriteriontable.h inputcriteriontable.cpp globals.h numberentry.h thresholdbox.h
rankcomboboxset.o: rankcomboboxset.cpp rankcomboboxset.h
measurementstable.o: measurementstable.h measurementstable.cpp globals.h numberentry.h changetrackable.h rankcomboboxset.h
changetrackable.o: changetrackable.h changetrackable.cpp
cardinalpreferencestable.o: cardinalpreferencestable.h cardinalpreferencestable.cpp numberentry.h changetrackable.h \
			globals.h
ordinalpreferencestable.o: ordinalpreferencestable.h ordinalpreferencestable.cpp rankcomboboxset.h changetrackable.h \
			globals.h

csmaa: $(objs)
	$(CXX) $(objs) $(libs) -o $@
clean:
	-rm -f $(objs) *~ csmaa
.PHONY: csmaa clean backup
