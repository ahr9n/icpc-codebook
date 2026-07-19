#!/bin/bash

set -e

rm -f *.aux *.log *.toc *.pdf

python3 build-contents.py > contents.tex

pdflatex -interaction=nonstopmode codebook.tex && pdflatex -interaction=nonstopmode codebook.tex

rm -f *.aux *.log *.toc contents.tex
