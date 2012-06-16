all:
	#motion-estimation
	make --directory=Motion_Estimation all
	
	#corner-detection
	make --directory=corner-detection all

	#rotation
	make --directory=Rotation all

	#scaling
	cd scaling; ./compile.sh; cd -

	#gaussian-blur
	make --directory=gaussianblur all

	#high-pass-filter
	cd HPF_Overlay; ./compile.sh; cd -