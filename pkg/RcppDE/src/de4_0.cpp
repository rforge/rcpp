// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Port of DEoptim (2.0.7) to Rcpp/RcppArmadillo/Armadillo
// Copyright (C) 2010  Dirk Eddelbuettel <edd@debian.org>
//
// DEoptim is Copyright (C) 2009 David Ardia and Katharine Mullen
// and based on DE-Engine v4.0, Rainer Storn, 2004  
// (http://www.icsi.berkeley.edu/~storn/DeWin.zip)

#include <RcppArmadillo.h>

RcppExport SEXP DEoptimC(SEXP lower, SEXP upper, SEXP fn, SEXP control, SEXP rho);
void devol(double VTR, double f_weight, double fcross, int i_bs_flag, 
           arma::rowvec & lower, arma::rowvec & upper, SEXP fcall, SEXP rho, int i_trace,
           int i_strategy, int i_D, int i_NP, int i_itermax,
           arma::rowvec & initpopv, int i_storepopfreq, int i_storepopfrom,
           int i_specinitialpop, int i_check_winner, int i_av_winner,
           arma::mat    & ta_popP, arma::mat    & ta_oldP, arma::mat    & ta_newP, arma::rowvec & t_bestP, 
	   arma::rowvec & ta_popC, arma::rowvec & ta_oldC, arma::rowvec & ta_newC, double       & t_bestC,	
           arma::rowvec & t_bestitP, arma::rowvec & t_tmpP, arma::rowvec & tempP,
           arma::rowvec & d_pop, arma::rowvec & d_storepop, arma::mat & d_bestmemit, arma::rowvec & d_bestvalit,
           int & i_iterations, double i_pPct, long & l_nfeval);
void permute(int ia_urn2[], int i_urn2_depth, int i_NP, int i_avoid, int ia_urntmp[]);
RcppExport double evaluate(long & l_nfeval, const arma::rowvec & param, SEXP par, SEXP fcall, SEXP env);
//RcppExport double evaluate(long & l_nfeval, const double *param, SEXP par, SEXP fcall, SEXP env);

RcppExport SEXP DEoptimC(SEXP lowerS, SEXP upperS, SEXP fnS, SEXP controlS, SEXP rhoS) {
    BEGIN_RCPP ;	// macro to fill in try part of try/catch exception handler

    Rcpp::Function fn(fnS);						// function to mininise
    Rcpp::Environment rho(rhoS); 					// environment to do it in
    Rcpp::NumericVector f_lower(lowerS), f_upper(upperS); 		// User-defined bounds
    Rcpp::List          control(controlS); 				// named list of params

    double VTR           = Rcpp::as<double>(control["VTR"]);		// value to reach
    int i_strategy       = Rcpp::as<int>(control["strategy"]);    	// chooses DE-strategy
    int i_itermax        = Rcpp::as<int>(control["itermax"]);		// Maximum number of generations
    long l_nfeval        = 0;//Rcpp::as<int>(control["nfeval"]);	// number of function evaluations    
    int i_D              = Rcpp::as<int>(control["npar"]);		// Dimension of parameter vector
    int i_NP             = Rcpp::as<int>(control["NP"]);		// Number of population members
    int i_storepopfrom   = Rcpp::as<int>(control["storepopfrom"]) - 1;  // When to start storing populations 
    int i_storepopfreq   = Rcpp::as<int>(control["storepopfreq"]);  	// How often to store populations 
    int i_specinitialpop = Rcpp::as<int>(control["specinitialpop"]);  	// User-defined inital population 
    Rcpp::NumericVector initialpopv = Rcpp::as<Rcpp::NumericVector>(control["initialpop"]);
    double f_weight      = Rcpp::as<double>(control["F"]);  		// stepsize 
    double f_cross       = Rcpp::as<double>(control["CR"]);  		// crossover probability 
    int i_bs_flag        = Rcpp::as<int>(control["bs"]);   		// Best of parent and child 
    int i_trace          = Rcpp::as<int>(control["trace"]);  		// Print progress? 
    int i_check_winner   = Rcpp::as<int>(control["checkWinner"]); 	// Re-evaluate best parameter vector? 
    int i_av_winner      = Rcpp::as<int>(control["avWinner"]);  	// Average 
    double i_pPct        = Rcpp::as<double>(control["p"]); 		// p to define the top 100p% best solutions 

    arma::rowvec minbound(f_lower.begin(), f_lower.size(), false); 	// convert three Rcpp vectors to arma vectors
    arma::rowvec maxbound(f_upper.begin(), f_upper.size(), false);
    arma::rowvec initpopv(initialpopv.begin(), initialpopv.size(), false);

    arma::mat ta_popP(i_NP*2, i_D);    					// Data structures for parameter vectors 
    arma::mat ta_oldP(i_NP,   i_D);
    arma::mat ta_newP(i_NP,   i_D);
    arma::rowvec t_bestP(i_D); 

    arma::rowvec ta_popC(i_NP*2);  				    	// Data structures for obj. fun. values associated with par. vectors 
    arma::rowvec ta_oldC(i_NP);
    arma::rowvec ta_newC(i_NP);
    double t_bestC; 

    arma::rowvec t_bestitP(i_D);
    arma::rowvec t_tmpP(i_D); 
    arma::rowvec tempP(i_D);

    int i_nstorepop = ceil((i_itermax - i_storepopfrom) / i_storepopfreq);
    arma::rowvec d_pop(i_NP*i_D); 
    arma::rowvec d_storepop(i_NP*i_D*i_nstorepop); 
    arma::mat d_bestmemit(i_itermax, i_D);       
    arma::rowvec d_bestvalit(i_itermax); 	 
    int i_iter = 0;

    // call actual Differential Evolution optimization given the parameters
    devol(VTR, f_weight, f_cross, i_bs_flag, minbound, maxbound, Rcpp::wrap(fn), Rcpp::wrap(rho), i_trace,
	  i_strategy, i_D, i_NP, i_itermax, initpopv, i_storepopfrom, i_storepopfreq, i_specinitialpop, i_check_winner, i_av_winner,
	  ta_popP, ta_oldP, ta_newP, t_bestP, ta_popC, ta_oldC, ta_newC, t_bestC, t_bestitP, t_tmpP, tempP,
	  d_pop, d_storepop, d_bestmemit, d_bestvalit, i_iter, i_pPct, l_nfeval);

    // and return a named list to R
    return Rcpp::List::create(Rcpp::Named("bestmem")   = Rcpp::wrap(t_bestP),		// sexp_bestmem,
			      Rcpp::Named("bestval")   = t_bestC,       // sexp_bestval,
			      Rcpp::Named("nfeval")    = l_nfeval,   	// sexp_nfeval,
			      Rcpp::Named("iter")      = i_iter,	// sexp_iter,
			      Rcpp::Named("bestmemit") = Rcpp::wrap(d_bestmemit), 	// sexp_bestmemit,
			      Rcpp::Named("bestvalit") = Rcpp::wrap(d_bestvalit),	// sexp_bestvalit,
			      Rcpp::Named("pop")       = Rcpp::wrap(d_pop),    		// sexp_pop,
			      Rcpp::Named("storepop")  = Rcpp::wrap(d_storepop));   	// sexp_storepop)
    END_RCPP   // macro to fill in catch() part of try/catch exception handler
}


void devol(double VTR, double f_weight, double f_cross, int i_bs_flag,
           arma::rowvec & fa_minbound, arma::rowvec & fa_maxbound, SEXP fcall, SEXP rho, int i_trace,
           int i_strategy, int i_D, int i_NP, int i_itermax,
           arma::rowvec & initialpopv, int i_storepopfrom, int i_storepopfreq, 
           int i_specinitialpop, int i_check_winner, int i_av_winner,
           arma::mat &ta_popP, arma::mat &ta_oldP, arma::mat &ta_newP, 
	   arma::rowvec & t_bestP, arma::rowvec & ta_popC, arma::rowvec & ta_oldC, arma::rowvec & ta_newC, 
	   double & t_bestC,
           arma::rowvec & t_bestitP, arma::rowvec & t_tmpP, arma::rowvec & tempP,
           arma::rowvec &d_pop, arma::rowvec &d_storepop, arma::mat & d_bestmemit, arma::rowvec & d_bestvalit,
           int & i_iterations, double i_pPct, long & l_nfeval) {

    const int urn_depth = 5;   			// 4 + one index to avoid 
    Rcpp::NumericVector par(i_D);  		// initialize parameter vector to pass to evaluate function 
    int i, j, k, i_r1, i_r2, i_r3, i_r4;  	// counting variables and placeholders for random indexes
    
    int ia_urn2[urn_depth];
    Rcpp::IntegerVector ia_urntmp(i_NP); 	// so that we don't need to re-allocated each time in permute

    int i_nstorepop = ceil((i_itermax - i_storepopfrom) / i_storepopfreq);
    int i_xav, popcnt, bestacnt, same; 		// lazy cnters 
    double f_jitter, f_dither, t_bestitC, t_tmpC, tmp_best; 
    
    arma::mat initialpop(i_NP, i_D); 

    int i_pbest;    				// vars for DE/current-to-p-best/1 
    int p_NP = round(i_pPct * i_NP);  		// choose at least two best solutions 
    p_NP = p_NP < 2 ? 2 : p_NP;
    arma::irowvec sortIndex(i_NP); 		// sorted values of ta_oldC 
    for(i = 0; i < i_NP; i++) sortIndex[i] = i;

    int i_len, done, step, bound;    		// vars for when i_bs_flag == 1 */
    double tempC;

    GetRNGstate();

    ta_popP.at(0,0) = 0;
    
    initialpop.zeros();		 		// initialize initial popuplation 
    d_bestmemit.zeros();    			// initialize best members
    d_bestvalit.zeros();			// initialize best values 
    d_pop.zeros();				// initialize best population
    d_storepop.zeros();				// initialize stored populations 
    i_nstorepop = (i_nstorepop < 0) ? 0 : i_nstorepop;
      
    if (i_specinitialpop > 0) {    		// if initial population provided, initialize with values 
	k = 0;
	for (j = 0; j < i_D; j++) { 		// FIXME: should really have a matrix passed in ! 
	    for (i = 0; i < i_NP; i++) {
		initialpop.at(i,j) = initialpopv[k];
		k += 1;
	    }
	}
    }
    l_nfeval = 0;    				// number of function evaluations (this is an input via DEoptim.control, but we over-write it?) 

    for (i = 0; i < i_NP; i++) {		// ------Initialization-----------------------------
	if (i_specinitialpop <= 0) { 		// random initial member 
	    for (j = 0; j < i_D; j++) {
		ta_popP.at(i,j) = fa_minbound[j] + unif_rand() * (fa_maxbound[j] - fa_minbound[j]);
	    }
	} else { /* or user-specified initial member */
	    ta_popP.row(i) = initialpop.row(i);
	} 
	ta_popC[i] = evaluate(l_nfeval, ta_popP.row(i), par, fcall, rho);
	if (i == 0 || ta_popC[i] <= t_bestC) {
	    t_bestC = ta_popC[i];
	    //for (j = 0; j < i_D; j++)  
	    //	t_bestP[j] = ta_popP.at(i,j);
	    t_bestP = ta_popP.row(i);
	}
    }

    ta_oldP = ta_popP;				// ---assign pointers to current ("old") population---
    ta_oldC = ta_popC;
  
    int i_iter = 0;				// ------Iteration loop--------------------------------------------
    popcnt = 0;
    bestacnt = 0;
    i_xav = 1;
  
    while ((i_iter < i_itermax) && (t_bestC > VTR)) {    // main loop ====================================
	if (i_iter % i_storepopfreq == 0 && i_iter >= i_storepopfrom) {  	// store intermediate populations -- FIXME could be list (or arma::field) of matrices
	    for (i = 0; i < i_NP; i++) {
		for (j = 0; j < i_D; j++) {
		    d_storepop[popcnt] = ta_oldP.at(i,j);
		    popcnt++;
		}
	    }
	} /* end store pop */
      
	//for(j = 0; j < i_D; j++) {			// store the best member -- could also be a matrix of itermax * i_D
	//    d_bestmemit[bestacnt] = t_bestP[j];
	//    bestacnt++;
	//}
	d_bestmemit.row(i_iter) = t_bestP;	// store the best member

	d_bestvalit[i_iter] = t_bestC;		// store the best value 
	
	//for (j = 0; j < i_D; j++) t_bestitP[j] = t_bestP[j];
	t_bestitP = t_bestP;
	t_bestitC = t_bestC;
      
	i_iter++;
     
	f_dither = f_weight + unif_rand() * (1.0 - f_weight);	// ----computer dithering factor -----------------
      
	if (i_strategy == 6) {			// ---DE/current-to-p-best/1 -----------------------------------------------------
	    arma::rowvec temp_oldC = ta_oldC;					// create a copy of ta_oldC to avoid changing it 
	    rsort_with_index( temp_oldC.memptr(), sortIndex.begin(), i_NP );  	// sort temp_oldC to use sortIndex later 
	}

	for (i = 0; i < i_NP; i++) {		// ----start of loop through ensemble------------------------

	    t_tmpP = ta_oldP.row(i);		// t_tmpP is the vector to mutate and eventually select
	    t_tmpC = ta_oldC[i];

	    permute(ia_urn2, urn_depth, i_NP, i, ia_urntmp.begin()); /* Pick 4 random and distinct */
	    i_r1 = ia_urn2[1];  /* population members */
	    i_r2 = ia_urn2[2];
	    i_r3 = ia_urn2[3];
	    i_r4 = ia_urn2[4];
		
	    /*===Choice of strategy=======================================================*/
	    /*---classical strategy DE/rand/1/bin-----------------------------------------*/
	    if (i_strategy == 1) {
	  
		j = (int)(unif_rand() * i_D); /* random parameter */
		k = 0;
		do {
		    /* add fluctuation to random target */
		    t_tmpP[j] = ta_oldP.at(i_r1,j) + f_weight * (ta_oldP.at(i_r2,j) - ta_oldP.at(i_r3,j));
		    
		    j = (j + 1) % i_D;
		    k++;
		}while((unif_rand() < f_cross) && (k < i_D));

	    }
	    /*---DE/local-to-best/1/bin---------------------------------------------------*/
	    else if (i_strategy == 2) {
	 
		j = (int)(unif_rand() * i_D); /* random parameter */
		k = 0;
		do {
		    /* add fluctuation to random target */
		    
		    t_tmpP[j] = t_tmpP[j] + f_weight * (t_bestitP[j] - t_tmpP[j]) + f_weight * (ta_oldP.at(i_r2,j) - ta_oldP.at(i_r3,j));
		    j = (j + 1) % i_D;
		    k++;
		} while((unif_rand() < f_cross) && (k < i_D));

	    }
	    /*---DE/best/1/bin with jitter------------------------------------------------*/
	    else if (i_strategy == 3) {
	 	  
		j = (int)(unif_rand() * i_D); /* random parameter */
		k = 0;
		do {
		    /* add fluctuation to random target */
		    f_jitter = 0.0001 * unif_rand() + f_weight;
		    t_tmpP[j] = t_bestitP[j] + f_jitter * (ta_oldP.at(i_r1,j) - ta_oldP.at(i_r2,j));
	    
		    j = (j + 1) % i_D;
		    k++;
		} while((unif_rand() < f_cross) && (k < i_D));

	    }
	    /*---DE/rand/1/bin with per-vector-dither-------------------------------------*/
	    else if (i_strategy == 4) {
		  
		j = (int)(unif_rand() * i_D); /* random parameter */
		k = 0;
		do {
		    /* add fluctuation to random target */
		    t_tmpP[j] = ta_oldP.at(i_r1,j) + (f_weight + unif_rand()*(1.0 - f_weight))* (ta_oldP.at(i_r2,j) - ta_oldP.at(i_r3,j));
		    
		    j = (j + 1) % i_D;
		    k++;
		} while((unif_rand() < f_cross) && (k < i_D));

	    }
	    /*---DE/rand/1/bin with per-generation-dither---------------------------------*/
	    else if (i_strategy == 5) {
	  
		j = (int)(unif_rand() * i_D); /* random parameter */
		k = 0;
		do {
		    /* add fluctuation to random target */
		    t_tmpP[j] = ta_oldP.at(i_r1,j) + f_dither * (ta_oldP.at(i_r2,j) - ta_oldP.at(i_r3,j));
	    
		    j = (j + 1) % i_D;
		    k++;
		} while((unif_rand() < f_cross) && (k < i_D));
       
	    }
	    /*---DE/current-to-p-best/1 (JADE)--------------------------------------------*/
	    else if (i_strategy == 6) {

		/* select from [0, 1, 2, ..., (pNP-1)] */
		i_pbest = sortIndex[(int)(unif_rand() * p_NP)];
		
		j = (int)(unif_rand() * i_D); /* random parameter */
		k = 0;
		do {
		    /* add fluctuation to random target */
		    t_tmpP[j] = ta_oldP.at(i,j) + f_weight * (ta_oldP.at(i_pbest,j) - ta_oldP.at(i,j)) + f_weight * (ta_oldP.at(i_r1,j) - ta_oldP.at(i_r2,j));
	    
		    j = (j + 1) % i_D;
		    k++;
		}while((unif_rand() < f_cross) && (k < i_D));

	    }
	    /*---variation to DE/rand/1/bin: either-or-algorithm--------------------------*/
	    else {
	  
		j = (int)(unif_rand() * i_D); /* random parameter */
		k = 0;
		if (unif_rand() < 0.5) { /* differential mutation, Pmu = 0.5 */
		    do {
			/* add fluctuation to random target */
			t_tmpP[j] = ta_oldP.at(i_r1,j) + f_weight * (ta_oldP.at(i_r2,j) - ta_oldP.at(i_r3,j));
			
			j = (j + 1) % i_D;
			k++;
		    }while((unif_rand() < f_cross) && (k < i_D));
		}
		else {
		    /* recombination with K = 0.5*(F+1) -. F-K-Rule */
		    do {
			/* add fluctuation to random target */
			t_tmpP[j] = ta_oldP.at(i_r1,j) + 0.5 * (f_weight + 1.0) * (ta_oldP.at(i_r2,j) + ta_oldP.at(i_r3,j) - 2 * ta_oldP.at(i_r1,j));
			
			j = (j + 1) % i_D;
			k++;
		    }while((unif_rand() < f_cross) && (k < i_D));

		}
	    }/* end if (i_strategy ...*/
	
	    /*----boundary constraints, bounce-back method was not enforcing bounds correctly*/
	    for (j = 0; j < i_D; j++) {
		if (t_tmpP[j] < fa_minbound[j]) {
		    t_tmpP[j] = fa_minbound[j] +
			unif_rand() * (fa_maxbound[j] - fa_minbound[j]);
		}
		if (t_tmpP[j] > fa_maxbound[j]) {
		    t_tmpP[j] =  fa_maxbound[j] -
			unif_rand() * (fa_maxbound[j] - fa_minbound[j]);
		}
	    }

	    /*------Trial mutation now in t_tmpP-----------------*/
	    t_tmpC = evaluate(l_nfeval, t_tmpP, par, fcall, rho); 	    // Evaluate mutant in t_tmpP[]

	    /* note that i_bs_flag means that we will choose the
	     *best NP vectors from the old and new population later*/
	    if (t_tmpC <= ta_oldC[i] || i_bs_flag) {
		/* replace target with mutant */
		for (j = 0; j < i_D; j++) 
		    ta_newP.at(i,j) = t_tmpP[j];
		ta_newC[i] = t_tmpC;
		if (t_tmpC <= t_bestC) {
		    for (j = 0; j < i_D; j++) 
			t_bestP[j] = t_tmpP[j];
		    t_bestC = t_tmpC;
		}
	    } 
	    else {
		for (j = 0; j < i_D; j++) 
		    ta_newP.at(i,j) = ta_oldP.at(i,j);
		ta_newC[i] = ta_oldC[i];
	  
	    }
	} /* End mutation loop through pop. */
	
     
	if(i_bs_flag) {
	    /* examine old and new pop. and take the best NP members into next generation */
	    for (i = 0; i < i_NP; i++) {
		for (j = 0; j < i_D; j++) 
		    ta_popP.at(i,j) = ta_oldP.at(i,j);
		ta_popC[i] = ta_oldC[i];
	    }
	    for (i = 0; i < i_NP; i++) {
		for (j = 0; j < i_D; j++) 
		    ta_popP.at(i_NP+i,j) = ta_newP.at(i,j);
		ta_popC[i_NP+i] = ta_newC[i];
	    }
	    i_len = 2 * i_NP;
	    step = i_len;  /* array length */
	    while (step > 1) {
		step /= 2;   /* halve the step size */
		do {
		    done = 1;
		    bound  = i_len - step;
		    for (j = 0; j < bound; j++) {
			i = j + step + 1;
			if (ta_popC[j] > ta_popC[i-1]) {
			    for (k = 0; k < i_D; k++) 
				tempP[k] = ta_popP.at(i-1, k);
			    tempC = ta_popC[i-1];
			    for (k = 0; k < i_D; k++) 
				ta_popP.at(i-1,k) = ta_popP.at(j,k);
			    ta_popC[i-1] = ta_popC[j];
			    for (k = 0; k < i_D; k++) 
				ta_popP.at(j,k) = tempP[k];
			    ta_popC[j] = tempC;
			    done = 0; 
			    /* if a swap has been made we are not finished yet */
			}  /* if */
		    }  /* for */
		} while (!done);   /* while */
	    } /*while (step > 1) */
	    /* now the best NP are in first NP places in gta_pop, use them */
	    for (i = 0; i < i_NP; i++) {
		for (j = 0; j < i_D; j++) 
		    ta_newP.at(i,j) = ta_popP.at(i,j);
		ta_newC[i] = ta_popC[i];
	    }
	} /*i_bs_flag*/

	/* have selected NP mutants move on to next generation */
	for (i = 0; i < i_NP; i++) {
	    for (j = 0; j < i_D; j++) 
		ta_oldP.at(i,j) = ta_newP.at(i,j);
	    ta_oldC[i] = ta_newC[i];
	}
	/* check if the best stayed the same, if necessary */
	if(i_check_winner)  {
	    same = 1;
	    for (j = 0; j < i_D; j++)
		if (t_bestitP[j] != t_bestP[j]) {
		    same = 0;
		}
	    if(same && i_iter > 1)  {
		i_xav++;
		tmp_best = evaluate(l_nfeval, t_bestP, par, fcall, rho);			// if re-evaluation of winner 

		/* possibly letting the winner be the average of all past generations */
		if(i_av_winner)
		    t_bestC = ((1/(double)i_xav) * t_bestC) + ((1/(double)i_xav) * tmp_best) + (d_bestvalit[i_iter-1] * ((double)(i_xav - 2))/(double)i_xav);
		else
		    t_bestC = tmp_best;
	    }
	    else {
		i_xav = 1;
	    }
	}
	for (j = 0; j < i_D; j++) 
	    t_bestitP[j] = t_bestP[j];
	t_bestitC = t_bestC;

	if( i_trace > 0 ) {
	    if( (i_iter % i_trace) == 0 ) {
		Rprintf("Iteration: %d bestvalit: %f bestmemit:", i_iter, t_bestC);
		for (j = 0; j < i_D; j++)
		    Rprintf("%12.6f", t_bestP[j]);
		Rprintf("\n");
	    }
	}
    } /* end loop through generations */
    
    /* last population */
    k = 0;
    for (i = 0; i < i_NP; i++) {
	for (j = 0; j < i_D; j++) {
	    d_pop[k] = ta_oldP.at(i,j);      
	    k++;
	}
    }
    i_iterations = i_iter;

    PutRNGstate();
}

inline void permute(int ia_urn2[], int i_urn2_depth, int i_NP, int i_avoid, int ia_urn1[])
/********************************************************************
 ** Function       : void permute(int ia_urn2[], int i_urn2_depth)
 ** Author         : Rainer Storn (w/bug fixes contributed by DEoptim users)
 ** Description    : Generates i_urn2_depth random indices ex [0, i_NP-1]
 **                  which are all distinct. This is done by using a
 **                  permutation algorithm called the "urn algorithm"
 **                  which goes back to C.L.Robinson.
 ** Functions      : -
 ** Globals        : -
 ** Parameters     : ia_urn2       (O)    array containing the random indices
 **                  i_urn2_depth  (I)    number of random indices (avoided index included)
 **                  i_NP          (I)    range of indices is [0, i_NP-1]
 **                  i_avoid       (I)    is the index to avoid and is located in
 **                                       ia_urn2[0].
 ** Preconditions  : # Make sure that ia_urn2[] has a length of i_urn2_depth.
 **                  # i_urn2_depth must be smaller than i_NP.
 ** Postconditions : # the index to be avoided is in ia_urn2[0], so fetch the
 **                   indices from ia_urn2[i], i = 1, 2, 3, ..., i_urn2_depth.
 ** Return Value   : -
 *********************************************************************/
{
    GetRNGstate();

    int k = i_NP;
    int i_urn1 = 0;
    int i_urn2 = 0;
    for (int i = 0; i < i_NP; i++)
	ia_urn1[i] = i; /* initialize urn1 */

    i_urn1 = i_avoid;                      /* get rid of the index to be avoided and place it in position 0. */
    while (k > i_NP - i_urn2_depth) {       /* i_urn2_depth is the amount of indices wanted (must be <= NP) */
	ia_urn2[i_urn2] = ia_urn1[i_urn1]; /* move it into urn2 */
	ia_urn1[i_urn1] = ia_urn1[k-1];    /* move highest index to fill gap */
	k = k - 1;                         /* reduce number of accessible indices */
	i_urn2 = i_urn2 + 1;               /* next position in urn2 */
	i_urn1 = (int)(unif_rand() * k);   /* choose a random index */
    }

    PutRNGstate();
}