Usage
=====
To execute the program, follow the following usage guidelines
blockWidth refers to the width of the blocks used in the block-matching algorithm.
searchPadding refers to the search area around the each block. oRefer to the project
report for more information.
Default values:
blockWidth = 16
searchPadding = 8

	sequential
		./bin/motion-estimation-sequential input-file1 input-file2 [blockWidth] [searchPadding]
	parallel
		./bin/motion-estimation-parallel input-file1 input-file2 [blockWidth] [searchPadding]
