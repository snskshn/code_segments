.SUFFIXES : .tex .dvi 

TEX = latex

PSFILE = make.ps 
DVIFILE = make.dvi

$(PSFILE) : $(DVIFILE)
	dvips $(DVIFILE) -o

make.ps : make.dvi 
make.dvi : make.tex
