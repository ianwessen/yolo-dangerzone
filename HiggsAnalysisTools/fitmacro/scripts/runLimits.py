#!/usr/bin/env python

import sys
import os.path
import optparse
import fnmatch
import subprocess
import hwwlimits

indir  = os.getcwd()+'/datacards/'
outdir = os.getcwd()+'/limits/'
outdirsignif = os.getcwd()+'/significance/'
srcdir = os.getcwd()+'/src/'
logdir = os.getcwd()+'/log/'
def main():

    usage = '''usage: %prog [opts] dctag'''
    parser = optparse.OptionParser(usage)
    parser.add_option('-s','--stepping',dest='stepping',help='Switch stepping on ', action='store_true', default=False)
    parser.add_option('-1',dest='minuit1',help='Minuit ', action='store_true', default=False)
    parser.add_option('--mlfit',dest='mlfit',help='Run toys with mlfit to check fit bias', action='store_true', default=False)
    parser.add_option('-n',dest='dryrun',help='Dry run ', action='store_true', default=False)
    parser.add_option('-o',dest='observed',help='Observed only', action='store_true', default=False)
    parser.add_option('-S','--significance',dest='significance',help='Compute the expected significance instead of the limit ', action='store_true', default=False)
    parser.add_option('--prefix','-p',dest='prefix',help='prefix',default=None)
    parser.add_option('--twodsuffix','-t',dest='suffix',help='suffix',default='')
    parser.add_option('--queue','-q',dest='queue',help='run in batch in queue specified as option (default -q 8nh)', default='8nh')
    parser.add_option('-l', '--lumi'     , dest='lumi'        , help='Luminosity'                            , default=None   , type='float'   )
    parser.add_option('-y', '--year'     , dest='year'        , help='Year'                                  , default=None   , type='float'   )    

    (opt, args) = parser.parse_args()
    print 'Running with lumi',opt.lumi,' for ',opt.year,' data'

    tevstr='_8TeV'
    if opt.year==2011:
        tevstr='_7TeV'
    elif opt.year==20112012:
        tevstr='_78TeV'
    
    constraints = {
        '*':'--rMin=-5.0 --rMax=8.0'
    }

    if len(args) != 1:
        parser.error('One and only one datacard tag at the time')

    tag = args[0]

    if tag not in hwwlimits.dcnames['all']:
        parser.error('Wrong tag: '+', '.join(sorted(hwwlimits.dcnames['all'])))

    tmpl = 'hww-{lumi:.2f}fb.mH{mass}.{tag}_shape'+opt.suffix+tevstr+'.txt'
    masses = [115, 120, 125, 130, 135, 140, 150, 160, 170, 180]
    if opt.mlfit: masses = [125]

    if opt.prefix:
        os.chdir(opt.prefix)
        print 'Running in ',os.getcwd()

    if not os.path.exists(indir):
        print 'Error: datacard directory',indir,'not found'
        sys.exit(-1)

    allcards = [(mass,os.path.join(indir,tmpl.format(mass=mass, tag=tag, lumi=opt.lumi))) for mass in masses] 
    for mass,card in allcards:
        if not os.path.exists(card):
            print 'Error: missing datacard: '+card
            sys.exit(-1)

    os.system('mkdir -p '+outdir)

    tagname = 'HWW_'+tag+'_shape'+opt.suffix+tevstr
    for mass,card in allcards:
        exe  = 'combine '
        flags = ' -n %s -m %s %s'%(tagname,mass,card)
        if opt.stepping:
            flags = ' --minosAlgo stepping'+flags
        if opt.minuit1:
            flags = ' --minimizerAlgo Minuit'+flags
        if opt.significance:
            if opt.observed:
                flags = ' -M ProfileLikelihood --significance '+flags
            else:
                flags = ' -M ProfileLikelihood --significance --expectSignal=1 -t 20 -s -1 '+flags
        elif opt.mlfit:
            flags = ' -M MaxLikelihoodFit --expectSignal=1 --saveNormalizations --noErrors --minos none -t 20 -s -1 '+flags
        else:
            flags = ' -M Asymptotic -s -1 '+flags
        if not opt.significance:
            for c,flag in constraints.iteritems():
                if fnmatch.fnmatch(str(mass),c):
                    flags = ' '+flag+flags

        command = exe+flags

        print '-'*50
        print command
        if not opt.queue: code = os.system(command)
        if opt.significance:
            move = 'mv higgsCombine%s.ProfileLikelihood.mH%d*.root %s' % (tagname,mass,outdirsignif)
        else:
            move = 'mv higgsCombine%s.Asymptotic.mH%d*.root %s' % (tagname,mass,outdir)
        os.system('mkdir -p '+srcdir)
        os.system('mkdir -p '+logdir)
        jobsperpoint = 1 if (opt.observed or "Asymptotic" in flags) else (1000 if opt.mlfit else 100)
        for j in range(jobsperpoint):
            runfile = srcdir+'run-mh'+str(mass)+'-'+tagname+'-j'+str(j)+'.src'
            f = open(runfile, 'w')
            f.write('cd ~/workspace/hww2l2nu/CMSSW_5_3_3/\n')
            f.write('eval `scram ru -sh` \n')
            f.write('cd - \n')
            f.write('source "$CMSSW_BASE/src/HWWAnalysis/ShapeAnalysis/test/env.sh"'+'\n')
            f.write(command+'\n')
            if opt.mlfit:
                move = 'mv higgsCombine%s.MaxLikelihoodFit.mH%d*.root %s ; mv mlfit%s.root %s/mlfit.mh%d.job%d.root' % (tagname,mass,outdir,tagname,outdir,mass,j)
            f.write(move+'\n')
            f.close()
            bsub = 'bsub -q '+opt.queue+' -J mh'+str(mass)+opt.suffix+'-j'+str(j)+' -o '+logdir+'/job-mh'+str(mass)+tagname+'-j'+str(j)+'.log source '+runfile 
            print '   job # '+str(j)
            if not opt.dryrun: os.system(bsub)
            
        if not opt.queue and not opt.dryrun:
            if code:
                sys.exit(code)
                os.system(move)

if __name__ == '__main__':
    main()

