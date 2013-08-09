#!/usr/bin/env python
import optparse
import os.path
import subprocess
import hwwlimits

mypath = os.path.dirname(os.path.abspath(__file__))

# void PlotLimit(string  limitFiles   = "inputs/ana_ICHEP_limits_nj_shape7teV_cut8TeV.txt",
#            string  outputPrefix = "combined",
#            string  luminosity   = "5.1 fb^{-1} (8 TeV) + 4.9 fb^{-1} (7 TeV)",
#            Float_t mhmin        = 110,
#            Float_t mhmax        = 160,
#            Int_t   setLogx      = 0,
#            Int_t   setLogy      = 1,
#            string  title        = "H #rightarrow WW #rightarrow 2l2#nu",
#            Bool_t  drawObserved = 1,
#            Int_t   ratio        = 0,
#            string  format       = "pdf")
# {

#---
def loadAndCompile(macro,options='g'):
    import ROOT
    import os
    try:
        code = ROOT.gROOT.LoadMacro(macro+'+'+options)
    except RuntimeError:
        code = ROOT.gROOT.LoadMacro(macro+'++'+options)
    return code

def runTheShape():
    usage = 'usage: %prog -p prefix channel'
    parser = optparse.OptionParser(usage)
    parser.add_option('--twodsuffix','-t',dest='suffix',help='suffix',default='')
    parser.add_option('--prefix','-p',dest='prefix', help='prefix', default='.')
    parser.add_option('-l', '--lumi'     , dest='lumi'        , help='Luminosity'                            , default=None   , type='float'   )
    parser.add_option('-o',dest='observed',help='Observed only', action='store_true', default=False)
    parser.add_option('-S','--significance',dest='significance',help='Compute the expected significance instead of the limit ', action='store_true', default=False)
    parser.add_option('-y', '--year'     , dest='year'        , help='Year'                                  , default=None   , type='float'   )    
    (opt, args) = parser.parse_args()

    if not args:
        raise ValueError('Desired channels missing: check the usage')
    if args[0] in hwwlimits.dcnames:
        plots = hwwlimits.dcnames[args[0]]
    else:
        plots = args[:]
        
    if opt.prefix:
        os.chdir(opt.prefix)

    name = opt.prefix if opt.prefix[-1] != '/' else opt.prefix[:-1] 

    tevstr='_8TeV'
    if opt.year==2011:
        tevstr='_7TeV'
    elif opt.year==20112012:
        tevstr='_78TeV'

    macropath = os.path.join(os.path.dirname(mypath),'limitmacros')

    doObs = True if opt.observed else False

    import ROOT
    loadAndCompile(macropath+'/tdrstyle.C')
    if opt.significance:
        loadAndCompile(macropath+'/PlotSignificance.C')
    else:
        loadAndCompile(macropath+'/PlotLimit.C')

    pars = {
        'option' : '',
        'mypath' : mypath,
        'name' : name,
        'lumi' : opt.lumi
    }
    print pars
    os.system('mkdir -p plots')
    for plot in plots:
        print plot
        pars['option'] = plot

        ROOT.gROOT.SetBatch(True)
        ROOT.setTDRStyle()
        if opt.significance:
            ROOT.PlotSignificance(os.getcwd()+"/../significance/{option}_shape".format(**pars)+opt.suffix+tevstr+".summary",
                                  os.getcwd()+"/plots/{name}_{option}".format(**pars)+opt.suffix,
                                  "{lumi} fb^{{-1}}".format(**pars), 
                                  110, 600, 0, 1, 
                                  "H #rightarrow WW #rightarrow 2l2#nu",
                                  doObs, 0, 'pdf')
            ROOT.PlotSignificance(os.getcwd()+"/../significance/{option}_shape".format(**pars)+opt.suffix+tevstr+".summary",
                                  os.getcwd()+"/plots/{name}_{option}".format(**pars)+opt.suffix,
                                  "{lumi} fb^{{-1}}".format(**pars), 
                                  110, 600, 0, 1, 
                                  "H #rightarrow WW #rightarrow 2l2#nu",
                                  doObs, 0, 'png')
        else:
            ROOT.PlotLimit(os.getcwd()+"/../limits/{option}_shape".format(**pars)+opt.suffix+tevstr+".summary",
                           os.getcwd()+"/plots/{name}_{option}".format(**pars)+opt.suffix,
                           "{lumi} fb^{{-1}}".format(**pars), 
                           110, 600, 0, 1, 
                           "H #rightarrow WW #rightarrow 2l2#nu",
                           doObs, 0, 'pdf')
            ROOT.PlotLimit(os.getcwd()+"/../limits/{option}_shape".format(**pars)+opt.suffix+tevstr+".summary",
                           os.getcwd()+"/plots/{name}_{option}".format(**pars)+opt.suffix,
                           "{lumi} fb^{{-1}}".format(**pars), 
                           110, 600, 0, 1, 
                           "H #rightarrow WW #rightarrow 2l2#nu",
                           doObs, 0, 'png')
            
#     p.communicate()
#     os.system('; '.join(commands))
    
    
if __name__ == '__main__':
    runTheShape()

