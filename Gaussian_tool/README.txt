i) The Tool folder contains the following files :

	1. Sampler.c will create the whole list(L) for a given \sigma and n.(As described in the paper) 

	2. The L should be divided by into smaller lists l's. (This has to be done manually)

	3. Run run.sh to minimize. Adjust the number of files in the .sh.
	Remember to change !(Boolean invert for Espresso) to ~(Boolean invert for C).

	4. The run merge.sh to merge all the files and get the full sampler. (remember to replace b,c and put correct value for out[x] in the script)

ii) The Example folder contains files for a discret Gaussian distribution with sigma=2 as used in Falcon signature algorithm. 
