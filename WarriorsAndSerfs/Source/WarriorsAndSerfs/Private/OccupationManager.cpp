// Fill out your copyright notice in the Description page of Project Settings.

#include "OccupationManager.h"



FSubjectSuitabilityScore::FSubjectSuitabilityScore()
{}

FSubjectSuitabilityScore::FSubjectSuitabilityScore(ASubject* theSubject, float theScore)
{
	subject = theSubject;
	score = theScore;
}

FBuildingSubjectScores::FBuildingSubjectScores()
{}


AOccupationManager::AOccupationManager()
{}