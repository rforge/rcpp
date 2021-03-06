
## results here are edited after a second run following manual
## installation of a number of package missing in the first run

## loads 'res'
#load("~/svn/rcpp/testlogs/result-20130928-094524.RData")

## Good:  dput(as.character(subset(res, res==0, pkg)[,1,drop=TRUE]))
goodPkg <- c("accelerometry", "acer", "AdaptiveSparsity", "ALKr", "Amelia",
             "apcluster", "BayesComm", "bcp", "bfa", "bfp", "bifactorial",
             "blockcluster", "ccaPP", "cda", "cladoRcpp", "clogitL1", "clusteval",
             "ConConPiWiFun", "coneproj", "Delaporte", "ecp", "EpiContactTrace",
             "fastGHQuad", "FastPCS", "FastRCS", "FBFsearch", "fdaMixed",
             "forecast", "gRbase", "gRim", "growcurves", "GSE", "GUTS", "HLMdiag",
             "httpuv", "inarmix", "jaatha", "Kmisc", "maxent", "minqa", "MPTinR",
             "msgl", "multmod", "mvabund", "MVB", "NetSim", "NetworkAnalysis",
             "oem", "phom", "phylobase", "planar", "prospectr", "psgp", "Rankcluster",
             "RcppArmadillo", "RcppBDT", "rcppbugs", "RcppClassic", "RcppClassicExamples",
             "RcppCNPy", "RcppDE", "RcppEigen", "RcppExamples", "RcppGSL",
             "RcppOctave", "RcppProgress", "RcppRoll", "RcppSMC", "RcppXts",
             "rexpokit", "rforensicbatwing", "rgam", "RInside", "Rmalschains",
             "RMessenger", "rmgarch", "Rmixmod", "robustgam", "robustHD",
             "rococo", "rotations", "RProtoBuf", "RQuantLib", "RSNNS", "RSofia",
             "rugarch", "RVowpalWabbit", "SBSA", "sdcMicro", "sequences",
             "simFrame", "sparseHessianFD", "sparseLTSEigen", "SpatialTools",
             "stochvol", "survSNP", "tagcloud", "termstrc", "tmg", "transmission",
             "trustOptim", "unmarked", "VIM", "waffect", "WideLM", "wordcloud",
             "zic")

## Bad: dput(as.character(subset(res, res==1, pkg)[,1,drop=TRUE]))
badPkg <- c("ALDqr", "CDM", "classify", "climdex.pcic", "diversitree",
            "ExactNumCI", "fugeR", "Funclustering", "geiger", "GeneticTools",
            "gMWT", "GOsummaries", "KernSmoothIRT", "LaF", "lme4", "marked",
            "mets", "mirt", "miscF", "ngspatial", "openair", "orQA", "PReMiuM",
            "sdcTable", "sirt", "spacodiR", "surveillance", "TAM", "tbart")

bad4missing <- c("ALDqr",		# 'HyperbolicDist’
                 "CDM",			# sirt, TAM
                 "classify",		# ‘R2WinBUGS’ ‘R2jags’
		 "climdex.pcic",	# 'PCIct'
                 "diversitree",		# ‘deSolve’ ‘subplex’
                 "fugeR",		# snowfall
		 "Funclustering",	# fda
                 "geiger",		# ‘msm’ ‘subplex’ ‘deSolve’ ‘coda’ ‘ncbit’
                 "GeneticTools",	# ‘gMWT’ ‘snpStats’
                 "gMWT",		# ‘clinfun’
                 "GOsummaries",		# ‘gProfileR’ ‘limma’
                 "LaF",			# 'yaml'
                 "lme4",		# MEMSS
                 "marked",		# ‘optimx’ ‘coda’ ‘R2admb’
                 "mets",		# ‘lava’ ‘timereg’ ‘prodlim’
                 "mirt",		# ‘GPArotation’
                 "miscF",		# ‘MCMCpack’
                 "ngspatial",		# ‘batchmeans’
                 "openair",		# ‘latticeExtra’ ‘hexbin’ ‘RgoogleMaps’ ‘png’ ‘mapdata’
                 "orQA",		# ‘genefilter’
                 "PReMiuM",		# ‘clue’
                 "sdcTable",		# ‘lpSolveAPI’
                 "sirt",		# ‘pbivnorm’ ‘sfsmisc’
                 "spacodiR",		# ‘picante’
                 "surveillance",	# ‘spatstat’
                 "TAM",			# ‘tensor’ ‘sfsmisc’ ‘GPArotation’ ‘psych’
                 "tbart")		# ‘GISTools’

bad4rcpp <- c()				# Yay!

## these fail initially but can all be run with some extra effort
bad4notrcpp <-   c("ExactNumCI",	# needs BOOSTLIB defined, bad src/ file
                   "KernSmoothIRT")	# rgl failed, needs full x11 session
good <- length(goodPkg)
bad  <- length(bad4missing) + length(bad4rcpp) + length(bad4notrcpp)
stopifnot(all.equal(bad,length(badPkg)))  ## account for GeoBIO

cat("Good       ", good, "\n")
cat("Bad        ", bad, "\n")
cat("  RcppErr  ", length(bad4rcpp), "\n")
cat("  MissDep  ", length(bad4missing), "\n")
cat("  NotRcpp  ", length(bad4notrcpp), "\n")
cat("Total      ", good + bad, "\n")
cat("Error Pct  ", length(bad4rcpp) / (good + bad), "\n")
