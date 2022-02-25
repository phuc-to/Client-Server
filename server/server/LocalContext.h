/**
 @file LocalContext.h
 @authors Phuc T, Narissa T, Kristen K
 @date 3/10/22
 @version 1.0
 */

#pragma once
class LocalContext
{
public:
	int getThreadRpCount() { return threadRpcCount; }
private:
	int threadRpcCount;
};