#pragma once

#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, FColor::Red, true)
#define DRAW_LINE(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Cyan, true, -1, 0, 1.f)
#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 30.f, FColor::Black, true)
#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, true, -1, 0, 1.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 30.f, FColor::Black, true); \
	}
