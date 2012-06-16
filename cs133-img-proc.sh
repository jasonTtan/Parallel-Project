#!/bin/bash

if [ $# -lt 1 ]; then
	echo -e "Usage:"
	echo -e 	"\t$0 img-op img-op-args";
	echo -e "Where";
	echo -e 	"\timg-op = { motion-estimation[-parallel], corner-detection[-parallel], rotation[-parallel], scaling[-parallel], gaussian-blur[-parallel], high-pass-filter[-parallel] }"
	echo -e "and img-op-args are the arguments for that operation.";

	exit 0;

else

	if [ "$1" = "motion-estimation" ]; then
		if [ $# -lt 3 ]; then
			echo -e $1 "usage: ";
			echo -e 	"\t$0 $1 input-file1 input-file2 [blockWidth] [searchPadding]";

			exit 0;
		else
			./Motion_Estimation/bin/motion-estimation-sequential "${@:2}";
		fi

	elif [ "$1" = "motion-estimation-parallel" ]; then
		if [ $# -lt 3 ]; then
			echo -e $1 "usage: ";
			echo -e 	"\t$0 $1 input-file1 input-file2 [blockWidth] [searchPadding]";

			exit 0;
		else
			./Motion_Estimation/bin/motion-estimation-parallel "${@:2}";
		fi

	elif [ "$1" = "corner-detection" ]; then
		if [ $# -lt 3 ]; then
			echo -e $1 "usage: ";
			echo -e 	"\t$0 $1 input_file output_file [threshold]";

			exit 0;
		else
			./corner-detection/bin/corner-detection-sequential "${@:2}";
		fi

	elif [ "$1" = "corner-detection-parallel" ]; then
		if [ $# -lt 3 ]; then
			echo -e $1 "usage: ";
			echo -e 	"\t$0 $1 input_file output_file [threshold] [num_threads]";

			exit 0;
		else
			./corner-detection/bin/corner-detection-parallel "${@:2}";
		fi

	elif [ "$1" = "rotation" ]; then
		if [ $# -lt 3 ]; then
			echo -e $1 "usage: ";
			echo -e 	"\t$0 $1 image-file rotation-degrees";

			exit 0;
		else
			./Rotation/bin/rotate-seq "${@:2}";
		fi

	elif [ "$1" = "rotation-parallel" ]; then
		if [ $# -lt 3 ]; then
			echo -e $1 "usage: ";
			echo -e 	"\t$0 $1 image-file rotation-degrees";

			exit 0;
		else
			./Rotation/bin/rotate-parallel "${@:2}";
		fi

	elif [ "$1" = "scaling" ]; then
		if [ $# -lt 3 ]; then
			echo -e $1 "usage: ";
			echo -e 	"\t$0 $1 input scaleFactor";

			exit 0;
		else
			./scaling/bin/scalingSequential "${@:2}";
		fi

	elif [ "$1" = "scaling-parallel" ]; then
		if [ $# -lt 3 ]; then
			echo -e $1 "usage: ";
			echo -e 	"\t$0 $1 input scaleFactor numThreads";

			exit 0;
		else
			./scaling/bin/scalingParallel "${@:2}";
		fi

	elif [ "$1" = "gaussian-blur" ]; then
		if [ $# -lt 3 ]; then
			echo -e $1 "usage: ";
			echo -e 	"\t$0 $1 input_file output_file sigma";

			exit 0;
		else
			./gaussianblur/bin/gb-sequential "${@:2}";
		fi

	elif [ "$1" = "gaussian-blur-parallel" ]; then
		if [ $# -lt 3 ]; then
			echo -e $1 "usage: ";
			echo -e 	"\t$0 $1 input_file output_file sigma";

			exit 0;
		else
			./gaussianblur/bin/gb-parallel "${@:2}";
		fi

	elif [ "$1" = "high-pass-filter" ]; then
		if [ $# -lt 3 ]; then
			echo -e $1 "usage: ";
			echo -e 	"\t$0 $1 input.bmp output.bmp strength";
			echo -e "where strength = [1-10]"

			exit 0;
		else
			./HPF_Overlay/bin/projSequential "${@:2}";
		fi

	elif [ "$1" = "high-pass-filter-parallel" ]; then
		if [ $# -lt 3 ]; then
			echo -e $1 "usage: ";
			echo -e 	"\t$0 $1 input.bmp output.bmp strength numThreads";
			echo -e "where strength = [1-10]"

			exit 0;
		else
			./HPF_Overlay/bin/projParallel "${@:2}";
		fi

	fi
fi