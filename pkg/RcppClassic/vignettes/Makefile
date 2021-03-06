
## There is an old bug in texidvi that makes it not swallow the ~
## marker used to denote whitespace. This is actually due to fixing
## another bug whereby you could not run texidvi on directory names
## containing a tilde (as we happen to do for Debian builds of R
## alpha/beta/rc releases). The 'tilde' bug will go away as it
## reportedly has been squashed upstream but I am still bitten by it
## on Ubuntu so for now Dirk will insist on pdflatex and this helps.
whoami=$(shell whoami)

all: RcppClassic-unitTests.pdf RcppClassic-intro.pdf 

pdfclean:
	rm -f RcppClassic-intro.pdf RcppClassic-unitTests.pdf
clean:
	@rm -f *.tex *.bbl *.blg *.aux *.out *.log

setvars:
ifeq (${R_HOME},)
R_HOME=	$(shell R RHOME)
endif
RPROG=	$(R_HOME)/bin/R
RSCRIPT=$(R_HOME)/bin/Rscript

RcppClassic-unitTests.pdf:
	rm -fr unitTests-results/*
	$(RSCRIPT) --vanilla unitTests/RcppClassic-unitTests.R
	$(RPROG) CMD Sweave RcppClassic-unitTests.Rnw
	$(RSCRIPT) --vanilla -e "tools::texi2dvi( 'RcppClassic-unitTests.tex', pdf = TRUE, clean = TRUE )"
	rm -fr RcppClassic-unitTests.tex

RcppClassic-intro.pdf: RcppClassic-intro.Rnw
	$(RPROG) CMD Sweave RcppClassic-intro.Rnw
ifneq (,$(findstring edd,$(whoami)))
	pdflatex RcppClassic-intro.tex
	bibtex RcppClassic-intro
	pdflatex RcppClassic-intro.tex
	pdflatex RcppClassic-intro.tex
else
	$(RSCRIPT) --vanilla -e "tools::texi2dvi( 'RcppClassic-intro.tex', pdf = TRUE, clean = FALSE )"
endif
	rm -fr RcppClassic-intro.tex

