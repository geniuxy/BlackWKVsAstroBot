#pragma once

#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, FColor::Red, true)
#define DRAW_SPHERE_COLOR(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 8.f, 24, Color, false, 5.f)
#define DRAW_SPHERE_SINGLE_FRAME(Location) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, FColor::Red, false, -1.f)
#define DRAW_LINE(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Cyan, true, -1.f, 0, 1.f)
#define DRAW_LINE_SINGLE_FRAME(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Cyan, false, -1.f, 0, 1.f)
#define DRAW_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 30.f, FColor::Black, true)
#define DRAW_POINT_SINGLE_FRAME(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 30.f, FColor::Black, false, -1.f)
#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, true, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 30.f, FColor::Black, true); \
	}
#define DRAW_VECTOR_SINGLE_FRAME(StartLocation, EndLocation) if (GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 30.f, FColor::Black, false, -1.f); \
	}
