#!/usr/bin/env python

import sys
import os
import ROOT
from ROOT import *
import HWWAnalysis.Misc.odict as odict
import hwwlimits
import optparse
import re


 
tagname = 'comb_shape'
basepath = os.getcwd()+'/limits/'

masses = [115, 120, 125, 130, 135, 140, 150, 160, 170, 180]

samplename = os.listdir(basepath)
samplename.sort()

points = ['minus2sigma', 'minus1sigma', 'median', 'plus1sigma', 'plus2sigma','observed']

def openTFile(path, option=''):
    f =  ROOT.TFile.Open(path,option)
    if not f.__nonzero__() or not f.IsOpen():
        raise NameError('File '+path+' not open')
    return f

def closeTFile(f):
    f.Close()

def getTree( file, tree ):
    t = file.Get(tree)
    if not t.__nonzero__():
        raise NameError('Tree '+str(tree)+' doesn\'t exist in '+file.GetName())
    return t


def getValue(file, q, mass):
    ## -1: observed limit
    ## 0.5: median expected
    ## 0.16/0.84: +- 1 sigma
    ## 0.025/0.975: +- 2 sigma
    print "mass = "+str(mass)+" for q = "+str(q)
    cut = ''
    if q is 'median':
        cut = 'quantileExpected==0.5'
    if q is 'observed':
        cut = 'quantileExpected==-1.'
    if q is 'minus1sigma':
        cut = 'quantileExpected>0.15 && quantileExpected<0.17'
    if q is 'minus2sigma':
        cut = 'quantileExpected>0.022 && quantileExpected<0.027'
    if q is 'plus1sigma':
        cut = 'quantileExpected>0.83 && quantileExpected<0.86'
    if q is 'plus2sigma':
        cut = 'quantileExpected>0.97 && quantileExpected<0.98'
    cut += ' && mh=='+str(mass)
    
    tree = getTree(file,'limit')
    command = 'limit>>h'
    tree.Draw(command,cut,'goff')
    hist = gDirectory.Get("h")
    value = hist.GetMean()
    hist.Delete()
    return value

def printTable(file, table):
    ## latex table
    print >> file, r'\documentclass[a4paper]{article}'
    print >> file, r'\begin{document}'
    print >> file, r'\pagestyle{empty}'
    print >> file, r'\begin{tabular}{|c|cccc|}'
    print >> file, r'\hline'
    print >> file, r'Higgs mass [$\mathrm{GeV/c^2}$] & Observed & Median & $68\%$ Range & $95\%$ Range \\'
    print >> file, r'\hline'
    print >> file, r'\hline'
    for mass in table:
        print >> file, r' {mass} & {observed:.2f} & {median:.2f} & [{minus1sigma:.2f}, {plus1sigma:.2f}] & [{minus2sigma:.2f}, {plus2sigma:.2f}] \\'.format(mass=mass,**(table[mass]))
    print >> file, r'\hline'
    print >> file, r'\end{tabular}'
    print >> file, r'\end{document}'  

def main():
    usage = 'usage: %prog [dir] [cmd]'
    parser = optparse.OptionParser(usage)
    parser.add_option('--twodsuffix','-t',dest='suffix',help='suffix',default='')
    parser.add_option('-y', '--year'     , dest='year'        , help='Year'                                  , default=None   , type='float'   )    
    (opt, args) = parser.parse_args()
    
    if len(args) != 1:
        parser.error('One and only one datacard tag at the time')

    tag = args[0]

    tevstr='_8TeV'
    if opt.year==2011:
        tevstr='_7TeV'
    elif opt.year==20112012:
        tevstr='_78TeV'

    if tag not in hwwlimits.dcnames['all']:
        parser.error('Wrong tag: '+', '.join(sorted(hwwlimits.dcnames['all'])))

    tagname = tag+'_shape'+opt.suffix+tevstr

    print tagname

    table = odict.OrderedDict()
    
    if not os.path.exists(basepath+'/higgsCombineHWW_'+tagname+'.Asymptotic.root'):
        hadd = 'hadd -k '+basepath+'/higgsCombineHWW_'+tagname+'.Asymptotic.root '+basepath+'/higgsCombineHWW_'+tagname+'.Asymptotic.mH*.root'
        print hadd
        os.system(hadd)
    
    sample = basepath+'/higgsCombineHWW_'+tagname+'.Asymptotic.root'
    
    if not opt.suffix in sample:
        return
    if not '.root' in sample:
        return
    if not tagname in sample:
        return
    print sample

    for mass in masses:
        line = {}
        for point in points:
            f = openTFile(sample)
            value = getValue(f,point,mass)
            line[point] = value

        table[mass] = line

    latex = open(basepath+'/'+tagname+'.tex','w')
    printTable(latex,table)
    latex.close()


    ## summary file
    summary = open(basepath+'/'+tagname+'.summary', 'w')
    for mass in table:
        summary.write('{mass} {observed:.3f} 99 {median:.3f} {minus2sigma:.3f} {minus1sigma:.3f} {plus1sigma:.3f} {plus2sigma:.3f}\n'.format( mass=mass, **(table[mass]) ) )

    summary.close()

if __name__ == '__main__':
    main()







## hww-1.55fb.mH120.comb.txt
## Observed Limit: r < 3.5430
## Expected  2.5%: r < 1.2811
## Expected 16.0%: r < 1.7041
## Expected 50.0%: r < 2.3613
## Expected 84.0%: r < 3.2799
## Expected 97.5%: r < 4.3577


## hww-1.55fb.mH130.comb.txt
## Observed Limit: r < 1.7859
## Expected  2.5%: r < 0.6184
## Expected 16.0%: r < 0.8226
## Expected 50.0%: r < 1.1398
## Expected 84.0%: r < 1.5832
## Expected 97.5%: r < 2.1035
