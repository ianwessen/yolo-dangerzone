#! /usr/bin/env python
import os
import sys
import time
import commands

import re
from math import *

from optparse import OptionParser
parser = OptionParser(usage="%prog [options]")
parser.add_option("-M", "--mc",      dest="mcYields", action="store_true", default=False, help="Use only MC-driven backgrounds");
parser.add_option("-A", "--asimov",  dest="asimov",   action="store_true", default=False, help="Replace observation with expected outcome (always on if lumi != refLumi)");
parser.add_option("-L", "--refLumi", dest="refLumi",  type="string", default="4.63fb", help="Luminosity of the inputs (XXXpb or XXXfb)");
parser.add_option("-l", "--lumi",    dest="lumi",     type="string", default=None,    help="Output luminosity (by default equal to the reference one)");
(options, args) = parser.parse_args()

def readMaster(file):
    map = {}
    for line in open(file,"r"):
        fields = [x.strip() for x in line.split()]
        if len(fields) < 2: continue
        if not map.has_key(int(fields[0])): map[int(fields[0])] = {}
        mapm = map[int(fields[0])]
        if not mapm.has_key(fields[2]): mapm[fields[2]] = {}
        mapp = mapm[fields[2]]
        mapp[int(fields[1])] = dict(zip(['mm','me','em','ee'],[ [float(y)] for y in fields[3:]]));
#         mapp[int(fields[1])] = dict(zip(['mm','me','em','ee'],[ [float(x),float(y)] for (x,y) in zip(fields[3::2],fields[4::2]) ]));
    return map

YieldTable = readMaster("mcyields.txt");

def file2map(file):
    map = {}
    for line in open(file,"r"):
        fields = [x.strip() for x in line.split()]
        if len(fields) > 1 and re.match("\d+", fields[0]):
            map[float(fields[0])] = [float(y) for y in fields[1:]]
    return map

def setDDyields(process, masterTable, filePattern, type, channels=['mm','me','em','ee'], jets=[0,1], maxM=9999):
    ret = {}; map0 = {}
    for j in jets:
        for c in channels:
            map0[(j,c)] = file2map(filePattern % (c,j))
    for m in masterTable.keys():
        if m >= maxM: continue
        mp = masterTable[m][process]
        for j in jets:
            for c in mp[j].keys():
                if c not in channels:
                    if YieldTable[m][process][j][c] !=0: mp[j][c] = YieldTable[m][process][j][c]
                    else: mp[j][c] = None
                elif type == "gamma":
                    mp[j][c] = [ map0[(j,c)][m][0] * map0[(j,c)][m][1]] + map0[(j,c)][m][:]
                elif type == "lnN":
                    mp[j][c] = map0[(j,c)][m][:]
                    mp[j][c][1] = 1 + mp[j][c][1]/mp[j][c][0] if mp[j][c][0] else 1.0
                            
if options.mcYields == False:
    setDDyields("WW", YieldTable, "WWCard_%s_%dj.txt", "gamma", maxM=200)
    setDDyields("ggWW", YieldTable, "ggWWCard_%s_%dj.txt", "gamma", maxM=200)
    setDDyields("DY", YieldTable, "DYCard_%s_%dj.txt", "gamma", channels=['mm','ee'])
    setDDyields("WJet", YieldTable, "WJet_%s_%dj.txt", "lnN")
    setDDyields("Top", YieldTable, "TopCard_%s_%dj.txt", "gamma")
    for m in YieldTable.keys():
        mp = YieldTable[m]['Top']
    VGammaTable = readMaster("vgamma-yields-mc.txt");
    for m in VGammaTable.keys():
        YieldTable[m]['Vg'] = VGammaTable[m]['vgamma']
        

YR_ggH = file2map("YR-ggH.txt")
YR_vbfH = file2map("YR-vbfH.txt")
ggH_pdfErrYR  = dict([(m, sqrt((1+0.01*pdf_hi)/(1+0.01*pdf_lo))) for m,(xs,xs_hi,xs_lo,sca_hi,sca_lo,pdf_hi,pdf_lo) in YR_ggH.items()] )
ggH_scaErrYR  = dict([(m, sqrt((1+0.01*sca_hi)/(1+0.01*sca_lo))) for m,(xs,xs_hi,xs_lo,sca_hi,sca_lo,pdf_hi,pdf_lo) in YR_ggH.items()] )
vbfH_pdfErrYR = dict([(m, sqrt((1+0.01*pdf_hi)/(1+0.01*pdf_lo))) for m,(xs,xs_hi,xs_lo,sca_hi,sca_lo,pdf_hi,pdf_lo) in YR_vbfH.items()] )
vbfH_scaErrYR = dict([(m, sqrt((1+0.01*sca_hi)/(1+0.01*sca_lo))) for m,(xs,xs_hi,xs_lo,sca_hi,sca_lo,pdf_hi,pdf_lo) in YR_vbfH.items()] )
ggH_jets = dict([(m, dict(zip(['f0','f1','f2','k1','k2'], vals))) for m,vals in file2map("ggH_jetBins.txt").items()]) 

scalef = 1.0
if   options.lumi == None: options.lumi = options.refLumi
elif options.lumi != options.refLumi:
    if   "pb" in options.lumi: scalef = 0.001*float(options.lumi.replace("pb",""))
    elif "fb" in options.lumi: scalef = 1.000*float(options.lumi.replace("fb",""))
    else: raise RuntimeError, "Option --lumi should be XXXpb or XXXfb, not '%s'" % options.lumi
    if   "pb" in options.refLumi: scalef /= 0.001*float(options.refLumi.replace("pb",""))
    elif "fb" in options.refLumi: scalef /= 1.000*float(options.refLumi.replace("fb",""))
    else: raise RuntimeError, "Option --refLumi should be XXXpb or XXXfb, not '%s'" % options.refLumi
    print "Extrapolation factor: ",scalef
    options.asimov = True

for m in YieldTable.keys():
    for j in 0,1:
        for c in ['mm','me','em','ee']:
            thisch = {}
            for p in YieldTable[m].keys(): 
                if YieldTable[m][p][j][c] == None: continue
                if YieldTable[m][p][j][c][0] == 0 and p != 'data':
                    if len(YieldTable[m][p][j][c]) <= 3: continue # constants or lnN with zero value
                    if len(YieldTable[m][p][j][c]) == 4 and YieldTable[m][p][j][c][2] == 0: continue # gamma with zero alpha
                if scalef != 1.0 and p == 'data': continue
                thisch[p] = YieldTable[m][p][j][c]
            print "Assembling card for mH = %d, channel %s, %d jets" % (m,c,j)
            card = open("hww-%s.mH%d.%s_%dj.txt" % (options.lumi,m,c,j), "w")
            card.write("## H->WW for m(H) = %d, channel %s, %d jets. Luminosity %s\n" % (m,c,j,options.lumi))
            if scalef != 1: card.write("## Taken extrapolating the %s analysis by a factor %.1f\n"%(options.refLumi,scalef))
            card.write("## Debug info: %s\n" % thisch)
            card.write("imax 1\njmax *\nkmax *\n");
            card.write(("-"*100) + "\n")
            if options.asimov: card.write("# we put as 'observation' the expected background-only outcome\n")
            card.write("bin          %s_%dj\n" % (c,j));
            if options.asimov:
                card.write("observation  %3d\n" % int(0.5+sum([y[0]*scalef for p,y in thisch.items() if p != 'ggH' and p != 'vbfH'])))
            else:
                card.write("observation  %3d\n" % thisch['data'][0])
            card.write(("-"*100) + "\n")
            keyline = [ ((i+1 if p!='ggH' and p!='vbfH' else -i), p, y[0]*scalef) for i,(p,y) in enumerate(thisch.items()) if p != 'data']; 
            keyline.sort()
            card.write("bin                               " + "   ".join("  %s_%dj" % (c,j) for i,p,y in keyline) + "\n");
            card.write("process                           " + "   ".join(" %6s"     % p     for i,p,y in keyline) + "\n");
            card.write("process                           " + "   ".join("    % 3d" % i     for i,p,y in keyline) + "\n");
            card.write("rate                              " + "   ".join(" %6.3f"   % y     for i,p,y in keyline) + "\n");
            card.write(("-"*100) + "\n")
            # -- Systematics ---------------------"
            MCBG = ['ggH', 'vbfH', 'DTT', 'VV', 'Vg'] # backgrounds from MC
            if m >= 200: MCBG += ['WW', 'ggWW']
            if c == 'em' or c == 'me':
                MCBG += ['DY']
            nuisances = []
            # -- Luminosity ---------------------
            nuisances.append(['lumi',      ['lnN'], dict([(p,1.045) for p in MCBG])])
            # -- PDF ---------------------
            nuisances.append(['pdf_gg',    ['lnN'], { 'ggH':ggH_pdfErrYR[m], 'ggWW':(1.0 if m < 200 else 1.04) }])
            nuisances.append(['pdf_qqbar', ['lnN'], { 'vbfH':vbfH_pdfErrYR[m], 'VV':1.04, 'WW':(1.0 if m < 200 else 1.04) }])
            # -- Theory ---------------------
            if j == 0:
                # appendix D of https://indico.cern.ch/getFile.py/access?contribId=0&resId=0&materialId=0&confId=135333
                k0 = pow(ggH_scaErrYR[m],     1/ggH_jets[m]['f0'])
                k1 = pow(ggH_jets[m]['k1'], 1-1/ggH_jets[m]['f0']) # -f1-f2=f0-1
                nuisances.append(['QCDscale_ggH',     ['lnN'], { 'ggH':k0 }])
                nuisances.append(['QCDscale_ggH1in',  ['lnN'], { 'ggH':k1 }])
                if m >= 200:
                    nuisances.append(['QCDscale_WW',     ['lnN'], { 'WW': 1.042 }])
                    nuisances.append(['QCDscale_WW1in',  ['lnN'], { 'WW': 0.978 }])                    
            elif j == 1:
                k1 = pow(ggH_jets[m]['k1'], 1+ggH_jets[m]['f2']/ggH_jets[m]['f1']) 
                k2 = pow(ggH_jets[m]['k2'],  -ggH_jets[m]['f2']/ggH_jets[m]['f1']) 
                nuisances.append(['QCDscale_ggH1in',  ['lnN'], { 'ggH':k1 }])
                nuisances.append(['QCDscale_ggH2in',  ['lnN'], { 'ggH':k2 }])
                if m >= 200:
                    nuisances.append(['QCDscale_WW1in', ['lnN'], {'WW': 1.076 }])
                    nuisances.append(['QCDscale_WW2in', ['lnN'], {'WW': 0.914 }])
            nuisances.append(['QCDscale_qqH',  ['lnN'], { 'vbfH':vbfH_scaErrYR[m] }])
            nuisances.append(['QCDscale_VV', ['lnN'], {'VV':1.03}])
            if thisch.has_key('Vg'): nuisances.append(['QCDscale_Vg', ['lnN'], {'Vg':1.30}])
            # -- Experimental ---------------------
            if 'm' in c: nuisances.append(['CMS_eff_m', ['lnN'], dict([(p,pow(1.02,c.count('m'))) for p in MCBG])])
            if 'e' in c: nuisances.append(['CMS_eff_e', ['lnN'], dict([(p,pow(1.02,c.count('e'))) for p in MCBG if p != 'Vg'])])
            if   c == 'mm': nuisances.append(['CMS_p_scale_m', ['lnN'], dict([(p,1.015) for p in MCBG if p != 'DTT'] )])
            elif c == 'em': nuisances.append(['CMS_p_scale_m', ['lnN'], dict([(p,1.015) for p in MCBG if p != 'DTT'] )])
            elif c == 'me': nuisances.append(['CMS_p_scale_e', ['lnN'], dict([(p,1.020) for p in MCBG if p != 'DTT'] )])
            elif c == 'ee': nuisances.append(['CMS_p_scale_e', ['lnN'], dict([(p,1.020) for p in MCBG if p != 'DTT'] )])
            nuisances.append(['CMS_met', ['lnN'], dict([(p,1.02) for p in MCBG if p != 'DTT'])])
            nuisances.append(['CMS_scale_j', ['lnN'], dict([(p,1.02) for p in MCBG if p != 'DTT'])])
            nuisances.append(['QCDscale_ggH_ACCEPT', ['lnN'], {'ggH':1.02}])
            nuisances.append(['QCDscale_qqH_ACCEPT', ['lnN'], {'vbfH':1.02}])
            if (j == 0): nuisances.append(['UEPS', ['lnN'], {'ggH':0.943}])
            else:        nuisances.append(['UEPS', ['lnN'], {'ggH':1.084}])
            if m>200: nuisances.append(['QCDscale_WW_EXTRAP', ['lnN'], {'WW':0.954}])
            # --- new ---
            nuisances.append(['theoryUncXS_HighMH', ['lnN'], { 'ggH':1+1.5*pow(float(m)/1000,3), 'vbfH':1+1.5*pow(float(m)/1000,3) } ])
            # -- Backgrounds ---------------------
            if options.mcYields:
                if c[-1] == "m" and thisch.has_key('WJet'):
                    nuisances.append(['FakeRate', ['lnN'], {'WJet':1.5}])
                if c[-1] == "e" and thisch.has_key('WJet'):
                    nuisances.append(['FakeRate', ['lnN'], {'WJet':1.5}])
                for X in ['Top', 'WW']: # 20%
                    if X == 'WW' and m >= 200: continue
                    if thisch.has_key(X): nuisances.append(['CMS_hww_%s'%X, ['lnN'], {X:1.2}])
                for X in ['DY']: # 100%
                    if thisch.has_key(X): nuisances.append(['CMS_hww_%s'%X, ['lnN'], {X:2.0}])
            else:
                if c[-1] == "m" and thisch.has_key('WJet'):
                    nuisances.append(['FakeRate', ['lnN'], {'WJet':thisch['WJet'][1]}])
                if c[-1] == "e" and thisch.has_key('WJet'):
                    nuisances.append(['FakeRate', ['lnN'], {'WJet':thisch['WJet'][1]}])
                if thisch.has_key('Vg'): nuisances.append(['CMS_fake_Vg', ['lnN'], {'Vg':2.0}])
                for X in ['Top', 'WW', 'ggWW']: # unique sideband, gamma + lnN
                    if (X == 'WW' or X == 'ggWW') and m >= 200: continue
                    if thisch.has_key(X):
                        nuisances.append(['CMS_hww_%s_%dj_extr'%(X,j), ['lnN'], {X:(1+thisch[X][3]/thisch[X][2])}])
                        nuisances.append(['CMS_hww_%s_%dj_stat'%(X,j), ['gmN', int(thisch[X][1]*scalef)], {X:thisch[X][2]}])
                for X in ['DY']: # two sidebands, gamma + gmM
                    if thisch.has_key(X) and (c == 'ee' or c == 'mm'):
                        nuisances.append(['CMS_hww_%s%s%dj_extr'%(X,c,j), ['lnN'], {X:(1.0+thisch[X][3]/thisch[X][2])}])
                        nuisances.append(['CMS_hww_%s%s%dj_stat'%(X,c,j), ['gmN', int(scalef*thisch[X][1])], {X:thisch[X][2]}])
            for (name,pdf,effect) in nuisances:
                if len(pdf) == 1: card.write("%-25s %3s     " % (name,pdf[0]))
                else:             card.write("%-25s %3s %3d " % (name,pdf[0],pdf[1]))
                for i,p,y in keyline:
                    if effect.has_key(p): 
                        if pdf[0] == 'gmN': card.write(" %6.4f" % effect[p])
                        else:               card.write("  %4.3f" % effect[p])
                    else:                   card.write("      -")
                    card.write("   ")
                card.write("\n");
            card.close()
