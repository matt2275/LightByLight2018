# MC data at /eos/cms/store/group/phys_top/gkrintir/LbL/GammaGamma2MuMu_GENSIM_woPtCut_PbPb5TeV_v1/crab_GammaGamma2MuMu_GENSIM_woPtCut/200317_235208/0000/
# Real Data at/eos/cms/store/group/phys_diffraction/lbyl_2018/HIForward_Reco/ntuples/ntuples_data/HIForward/ntuples_data_lbl/200617_140125/0000/data_HiForwardAOD_1-1.root

import sys


runfile = sys.argv[2]
config = "/afs/cern.ch/user/m/mnickel/private/LightByLight2018/analysis/configs/efficiencies.md"
input_file = sys.argv[1]

ending = input_file[(input_file.find("data_HiForwardAOD") + len("data_HiForwardAOD")):]

sig_mu = "/eos/user/m/mnickel/TauTau/Exclusivity/Data_Exc"
mu_ele = "/eos/user/m/mnickel/TauTau/Exclusivity/Data_Exc_Tracks"

f = open(runfile,"a")
f.write( config + "\n")
f.write( input_file+ "\n")
f.write( sig_mu + ending+ "\n")
f.write( mu_ele + ending + "\n")
f.close()
