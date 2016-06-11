#include <maya/MSimple.h>
#include <maya/MIOStream.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MPoint.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MObject.h>
#include <maya/MFnTransform.h>
#include <maya/MVector.h>
#include <maya/MTypes.h>

#include <random>
#include <cstdlib>
#include <string>
#include <math.h>
#include <ctgmath>
#include <vector>

DeclareSimpleCommand(BallDrop, "Spencer Fricke", "1.0");

double getRandom(int range, int offset) {
	return rand() % range + offset; 
}



MStatus BallDrop::doIt(const MArgList& args)
{
	//parameters for ball droping animation

	// Random range of 10 - 30
	int randRange = 50;
	int randOffset = 30;
	//max down bounce is 1 - .4 = .6
	double maxDownBounce = .6;
	double downBounceDivider = (randRange + randOffset) * (1 / ( 1 - maxDownBounce ) ) ; 
	double upBounceFrameOffset = 3.0;
	double downBounceFrameOffset = 2.0;
	double bounceReduce = .75;

	//the circle math to get balls in cluster
	double ringToRingSpacing = 1.0;
	double ballToBallSpacing = 3.0;



	MStatus stat;
	int	radius = 4; //default
	int NumOfSpheres = 1; //default
	unsigned i; //used for lafs
	//int ballCount = 0;

	double posY; 
	double frame;

	//default vector weights for moving balls around
	//only z is not changed, atm
	double xx = 0.0;
	double yy = 0.0;
	double zz = 0.0;

	// Parse the arguments.
	for (i = 0; i < args.length(); i++)
		if (MString("-q") == args.asString(i, &stat)
			&& MS::kSuccess == stat)
		{
			int tmp = args.asInt(++i, &stat);
			if (MS::kSuccess == stat)
				NumOfSpheres = tmp;
		}
		else if (MString("-r") == args.asString(i, &stat)
			&& MS::kSuccess == stat)
		{
			int tmp = args.asInt(++i, &stat);
			if (MS::kSuccess == stat)
				radius = tmp;
		}

	//creating the starting coords for the balls
	double ringRadius = 0;
	double ringCircum = 0;
	double ringCircumCopy;
	int ballsInRing = 0;
	double ballToBallSpaceAdjust;
	std::vector<MVector> startingCoordArray;
	int ballIndex = 1; //index 0 taken care off 
	startingCoordArray.push_back(MVector(0, 0, 0)); // first ball edge case

	while (ballIndex < NumOfSpheres) {
		ringRadius = ringRadius + (radius * 2) + ringToRingSpacing; //new radius
		ringCircum = 2 * ringRadius * M_PI;
		ringCircumCopy = ringCircum;
		//gets numbers of ball and space size for ring

		ballsInRing = 0; //resets balls in the ring
		while (ringCircumCopy > (radius + ballToBallSpacing)) {
			ringCircumCopy -= (radius + ballToBallSpacing);
			ballsInRing++; //this rings balls
			ballIndex++; //overall Balls
		}
		//ballToBallSpaceAdjust = ballToBallSpacing + (ballLeftInRing / ringCircumCopy);
		//x = rcos(θ) ~ y = rsin(θ)
		for (int i = 0; i < ballsInRing; i++) {
			xx = ringRadius * cos(i * (2 * M_PI / ballsInRing));
			zz = ringRadius * sin(i * (2 * M_PI / ballsInRing));
			startingCoordArray.push_back(MVector(xx, 0, zz));
		}
		
	}
	MSelectionList slist;
	MGlobal::getActiveSelectionList(slist);
	MDagPath        mDagPath;
	MObject         mComponent;

	//for each ball generated
	for (int i = 0; i < NumOfSpheres; i++) {

		//create the ball with given radius
		MString sphereMel = "polySphere -r ";
		sphereMel += (int)radius;
		//gives the ball a name
		sphereMel += " -n ballDrop";
		sphereMel += (int)i;

		//std::cout << sphereMel << std::endl;
		MGlobal::executeCommand(sphereMel);

		//gets the ball
		MGlobal::getActiveSelectionList(slist);
		mDagPath = MDagPath();
		slist.getDagPath(0, mDagPath);

		//moves it
		startingCoordArray[i].y = getRandom(randRange, randOffset) + radius;
		MFnTransform transFn(mDagPath, &stat);
		stat = transFn.translateBy(startingCoordArray[i], MSpace::kWorld);

		//sets key frames
		posY = startingCoordArray[i].y;
		frame = 0;

		MString animation = "";
		for (double maxY = posY; maxY >= radius + .2;) {
			//top of bounce
			animation += "setKeyframe -at translateY -t ";
			animation += frame;
			animation += " -v ";
			animation += maxY;

			animation += ";setKeyframe -at scaleY -t ";
			animation += frame;
			animation += " -v 1;";

						
			frame += (sqrt(6.116 * maxY)); //time = sqrt( 2 x 'height/m' / '9.8m/ss' * '30fps')
			
			//hitting ground
			animation += ";setKeyframe -at translateY -t ";
			animation += frame;
			animation += " -v ";
			animation += radius;
			animation += ";";

			animation += ";setKeyframe -at scaleY -t ";
			animation += (frame - downBounceFrameOffset);
			animation += " -v 1;";

			animation += ";setKeyframe -at scaleY -t ";
			animation += frame;
			animation += " -v ";
			animation += (1 - (maxY / downBounceDivider));
			animation += ";";

			animation += ";setKeyframe -at scaleY -t ";
			animation += (frame + upBounceFrameOffset);
			animation += " -v ";
			animation += (1 + (maxY / downBounceDivider / 2));
			animation += ";";

			animation += ";setKeyframe -at scaleY -t ";
			animation += (frame + (upBounceFrameOffset * 2));
			animation += " -v 1;";

			animation += "keyTangent - inTangentType linear - outTangentType linear -t ";
			animation += frame;
			animation += ";";

			MGlobal::executeCommand(animation);
			std::cout << animation << std::endl;
			animation = ""; //clears string buffer
			
			maxY *= bounceReduce;
			frame += (sqrt(6.116 * maxY));
		}
		
	
	}



	return stat;
}
