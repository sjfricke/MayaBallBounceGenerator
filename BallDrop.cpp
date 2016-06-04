//#include <math.h>
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
#include <string>
DeclareSimpleCommand(BallDrop, "Autodesk - Example", "3.0");
MStatus BallDrop::doIt(const MArgList& args)
{
	MStatus stat;
	int	radius = 4;
	int NumOfSpheres = 1;
	unsigned	i;
	int sphereCount = 0;

	double xx = 3.0;
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

	MVector spaceVector = MVector(xx, yy, zz);
	MSelectionList slist;
	MGlobal::getActiveSelectionList(slist);
	MDagPath        mDagPath;
	MObject         mComponent;

	for (int i = 0; i < NumOfSpheres; i++) {
		MString sphereMel = "polySphere -r ";
		sphereMel += (int)radius;

		sphereMel += " -n ballDrop";
		sphereMel += (int)sphereCount;

		//std::cout << sphereMel << std::endl;
		MGlobal::executeCommand(sphereMel);

		MGlobal::getActiveSelectionList(slist);

		mDagPath = MDagPath();
		slist.getDagPath(0, mDagPath);

		MFnTransform transFn(mDagPath, &stat);

		stat = transFn.translateBy(spaceVector, MSpace::kWorld);

		spaceVector.x += 3.0;
	}



	return stat;
}
