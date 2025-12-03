/***************************************************************************
This is to certify that this project is my own work, based on my personal 
efforts in studying and applying the concepts learned. I have constructed 
the functions and their respective algorithms and corresponding code by 
myself. The program was run, tested, and debugged by my own efforts. I 
further certify that I have not copied in part or whole or otherwise 
plagiarized the work of other students and/or persons.
Harvey Julian Ermitanio, DLSU ID# 12504610
***************************************************************************/

/*
Description: This program determines if a person is underweight, normal 
             weight, overweight, or obese based on BMI calculation and 
             suggests an appropriate exercise plan to achieve ideal weight.
Programmed by: Harvey Julian Ermitanio S12B
Last modified: November 24, 2025
Version: 1.0
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* Conversion Factors */
#define LBS_TO_KG 0.453592
#define FT_TO_IN 12
#define LBS_TO_CAL 3500

/* Specs Constraints */
#define MAX_MINUTES_PER_DAY 90

/* Screen Headers */
#define WELCOME          1
#define SETTINGS         2
#define CALCULATING      3
#define EXERCISE_PROGRAM 4
#define EXERCISE_LIST    5
#define MAIN             1

/* Input Validation */
#define INVALID 0
#define VALID   1

/* MAIN MENU OPTIONS */
#define START_RECO 1
#define SETTINGS   2
#define EXIT       3

/* SETTINGS OPTIONS */
#define SEE_EXERCISE_LIST 1
#define ADD_EXERCISE      2
#define REMOVE_EXERCISE   3
#define SELECT_ACTIVITIES 4
#define EXIT_SETTINGS     5

/* Line Separator Types */
#define EQUAL_SIGN 1
#define DASH       2

/* BMI Categories */
#define UNDERWEIGHT 0
#define NORMAL      1
#define OVERWEIGHT  2
#define OBESE       3

/* Height */
#define FIVE_FEET 60

/* Gender */
#define MALE 'm'
#define FEMALE 'f'

/* Activity Array */
#define MAX_ACTIVITIES 30

typedef struct
{
    char sName[30];
    float fMET;
    int bSelected; 
} structActivityType;

/*---------------Function Prototypes---------------*/

/* Conversion Functions */
int FTtoInches(int nHeightFT);
float LbsToKG(float fLbs);

/* UI Functions */
void displayHeader();
void displayLineSeparator(int nLength, int nType);
void displaySubHeader(int nTitle);
void displayExerciseList(structActivityType arrActivities[], int nSize);

/* Input Functions */
int askMenuOptions(int nScreenType);
int validateMenuOption(int nUserInput, int nScreenType);
void getUserData(char *cGender, float *fWeight, int *nHeightFt, int *nHeightIn);

/* Calculation Functions */
float calculateCaloriesBurned(float fMET, float fWeightKg, int nMinutes);
float computeBMI(float fWeight, int nHeightFt, int nHeightIn);
int categorizeBMI(float fBMI);
float calculateIdealWeight(int nHeightFt, int nHeightIn, char cGender);
void sortActsByMET(structActivityType arrActivities[], int nSize);
void allocateMinutesPerExercise(structActivityType arrActivities[], 
                                float fWeightLbs, float fIdealWeight,
                                int *nFirstActMinutes, float *fFirstActCalBurned, 
                                int *nSecondActIndex, float *fSecondActMinutes, 
                                float *fSecondActCalBurned, int nSize);

/* Display and Recommendation Functions */
void displayResults(structActivityType arrActivities[], float fWeightLbs, 
                    float fIdealWeight, int nCategory);
void recommendProgramUnderweight(structActivityType arrUnderweightActs[], 
                                 int nNumActs);

/* Settings Functions */
void addActivity(structActivityType arrActivities[], int *nNumActivities);
void removeActivity(structActivityType arrActivities[], int *nNumActivities);
void selectActivities(structActivityType arrActivities[], int nNumActivities);

/*---------------Function Implementations---------------*/

/* Converts feet to inches
   @param nHeightFT is the height in feet
   @return the height converted to inches
*/
int 
FTtoInches(int nHeightFT) 
{ 
	if (nHeightFT >= 0)
    	return nHeightFT * FT_TO_IN; 
    else
    {
    	printf("nHeightFT cannot be negative!\n");
    	return 0;
	}

}

/* Converts pounds to kilograms
   @param fLbs is the weight in pounds
   @return the weight converted to kilograms
*/
float 
LbsToKG(float fLbs)
{
	if (fLbs >= 0)
    	return fLbs * LBS_TO_KG;
    else
    {
    	printf("fLbs cannot be negative!\n");
    	return 0;
	}
}

/* Displays the main program header with ASCII art
*/
void 
displayHeader() 
{
    printf("  __  __       _          __  __        ______ _ _   \n");
    printf(" |  \\/  |     | |        |  \\/  |      |  ____(_) |  \n");
    printf(" | \\  / | __ _| | _____  | \\  / | ___  | |__   _| |_ \n");
    printf(" | |\\/| |/ _` | |/ / _ \\ | |\\/| |/ _ \\ |  __| | | __|\n");
    printf(" | |  | | (_| |   <  __/ | |  | |  __/ | |    | | |_ \n");
    printf(" |_|  |_|\\__,_|_|\\_\\___| |_|  |_|\\___| |_|    |_|\\__|\n");
    printf("\n");
}

/* Displays a line separator of specified length and type
   @param nLength is the number of characters to display
   @param nType is the type of separator (EQUAL_SIGN or DASH)
*/
void 
displayLineSeparator(int nLength, int nType)
{
    int nCtr;
    
    switch (nType)
    {
        case EQUAL_SIGN: 
            for (nCtr = 0; nCtr < nLength; nCtr++)
                printf("=");
            printf("\n");
            break;
            
        case DASH: 
            for (nCtr = 0; nCtr < nLength; nCtr++)
                printf("-");
            printf("\n");
            break;
        default:
        	printf("No such type. Please check displayLineSeparator() function");
        	break;
    }
}

/* Displays a sub-header based on the title type
   @param nTitle is the type of header to display
*/
void 
displaySubHeader(int nTitle)
{
    printf("\n");
    switch (nTitle)
    {
        case WELCOME:
            displayLineSeparator(34, EQUAL_SIGN);
            printf("     Welcome to Make Me Fit!\n");
            displayLineSeparator(34, EQUAL_SIGN);
            printf("\n");
            break;

        case SETTINGS:
            displayLineSeparator(34, EQUAL_SIGN);
            printf("            Settings\n");
            displayLineSeparator(34, EQUAL_SIGN);
            printf("\n");
            break;

        case CALCULATING:
            displayLineSeparator(34, EQUAL_SIGN);
            printf("           Calculating...\n");
            displayLineSeparator(34, EQUAL_SIGN);
            printf("\n");
            break;

        case EXERCISE_PROGRAM:
            displayLineSeparator(34, EQUAL_SIGN);
            printf("         Exercise Program\n");
            displayLineSeparator(34, EQUAL_SIGN);
            printf("\n");
            break;

        case EXERCISE_LIST:
            displayLineSeparator(34, EQUAL_SIGN);
            printf("         Exercise List\n");
            displayLineSeparator(34, EQUAL_SIGN);
            printf("\n");
            break;
        default:
        	printf("No such title. Please check displaySubHeader() function.\n");
        	break;
    }
}

/* Displays the list of available exercises with their MET values
   Precondition: arrActivities is properly initialized
   @param arrActivities is the array containing all exercises
   @param nSize is the size of the array
*/
void
displayExerciseList(structActivityType arrActivities[], int nSize)
{
    int nIndex, nDisplayNum = 0;
    
    displaySubHeader(EXERCISE_LIST);
    
    /* Loop through the array */
    for (nIndex = 0; nIndex < nSize; nIndex++)
    {
    	/* Ensures current element is not empty */
        if (arrActivities[nIndex].sName[0] != '\0' )
        {
        	nDisplayNum++;
            printf("  [%d] %s (%.1f MET)", nDisplayNum, 
                   arrActivities[nIndex].sName, arrActivities[nIndex].fMET);
            
            /* Show selection status */
            if (arrActivities[nIndex].bSelected)
                printf(" [SELECTED]");
            else
            	printf(" [NOT SELECTED]");
            
            printf("\n");
        }
    }
    displayLineSeparator(34, DASH);
}

/* Prompts and retrieves menu option from user
   @param nScreenType indicates which menu to display (MAIN or SETTINGS)
   @return the user's menu choice
*/
int
askMenuOptions(int nScreenType)
{
    int nUserInput;
    
    printf("\n");
    switch(nScreenType)
    {
        case MAIN:
            printf("[1] Start recommendation\n");
            printf("[2] Settings\n");
            printf("[3] Exit \n");
            printf("\nEnter option: ");
            scanf("%d", &nUserInput);
            break;
            
        case SETTINGS:
            printf("[1] See Current Exercise List\n");
            printf("[2] Add Exercise\n");
            printf("[3] Remove Exercise\n");
            printf("[4] Select Preferred Activities\n");
            printf("[5] Exit Settings\n");
            printf("\nEnter option: ");
            scanf("%d", &nUserInput);
            break;
            
        /* Ensures screen type is valid */
        default:
        	printf("No such screen type. Check askMenuOptions function.\n");
        	break;
    }

    return nUserInput;
}

/* Validates if the menu option entered is within valid range
   @param nUserInput is the option entered by the user
   @param nScreenType indicates which menu is being validated
   @return VALID if input is valid, INVALID otherwise
*/
int
validateMenuOption(int nUserInput, int nScreenType)
{
    switch (nScreenType)
    {
        case MAIN:
            if (nUserInput < 1 || nUserInput > 3)
            {
                printf("\nInvalid input. Please try again.");
                return INVALID;
            }
            break;
            
        case SETTINGS:
            if (nUserInput < 1 || nUserInput > 5)
            {
                printf("\nInvalid input. Please try again.");
                return INVALID;
            }
            break;
            
        /* Ensures screen type is valid */
        default:
        	printf("No such screen type. Please check validateMenuOption function. \n");
        	return INVALID;
        	break;
    }
    
    return VALID;
}

/* Prompts and retrieves user's personal data with input validation
   Precondition: All pointer parameters point to valid memory locations
   @param cGender is a pointer to store the user's gender
   @param fWeight is a pointer to store the user's weight in pounds
   @param nHeightFt is a pointer to store the user's height in feet
   @param nHeightIn is a pointer to store the user's height in inches
*/
void 
getUserData(char *cGender, float *fWeight, int *nHeightFt, int *nHeightIn)
{
    do 
    {
        printf("Enter your gender (M/F): ");
        scanf(" %c", cGender);
        
        if (*cGender != 'm' && *cGender != 'M' && 
            *cGender != 'f' && *cGender != 'F') 
        {
            printf("Invalid input. Please enter 'M' or 'F'.\n");
        }
        
        if (*cGender == 'M')
        {
            *cGender = MALE;
        }
        else if (*cGender =='F')
        {
            *cGender = FEMALE;
        }
        
    } while (*cGender != 'm' && *cGender != 'M' && 
             *cGender != 'f' && *cGender != 'F');
    
    do 
    {
        printf("Enter your weight (in lbs): ");
        scanf("%f", fWeight);
        
        if (*fWeight <= 0) 
        {
            printf("Invalid input. Weight must be greater than 0.\n");
        }
    } while (*fWeight <= 0);
    
    do 
    {
        printf("Enter your height (feet): ");
        scanf("%d", nHeightFt);
        
        if (*nHeightFt < 0) 
        {
            printf("Invalid input. Height in feet cannot be negative.\n");
        }
    } while (*nHeightFt < 0);
    
    do 
    {
        printf("Enter your height (inches): ");
        scanf("%d", nHeightIn);
        
        if (*nHeightIn < 0 || *nHeightIn >= 12) 
        {
            printf("Invalid input. Inches must be between 0 and 11.\n");
        }
    } while (*nHeightIn < 0 || *nHeightIn >= 12);
}

/* Calculates calories burned for a given activity
   Precondition: fMET, fWeightKg, and nMinutes are positive values
   @param fMET is the metabolic equivalent of the activity
   @param fWeightKg is the weight in kilograms
   @param nMinutes is the duration of activity in minutes
   @return the total calories burned
*/
float
calculateCaloriesBurned(float fMET, float fWeightKg, int nMinutes)
{
    float fResults = fMET * fWeightKg * nMinutes / 60;
    
    /* Ensures calories burned is not negative */
    if (fResults >= 0)
    	return fResults;
	else
		return 0;
}

/* Computes the Body Mass Index using the imperial formula
   Precondition: fWeight is positive, nHeightFt and nHeightIn are non-negative
   @param fWeight is the weight in pounds
   @param nHeightFt is the height in feet
   @param nHeightIn is the height in inches
   @return the calculated BMI value
*/
float
computeBMI(float fWeight, int nHeightFt, int nHeightIn)
{
    int nTotalIn = FTtoInches(nHeightFt) + nHeightIn;
    float fResults = (fWeight * 703) / (nTotalIn * nTotalIn);
    
    /* Ensures BMI is not negative */
    if (fResults >= 0)
    	return fResults;
    else
    	return 0;
}

/* Categorizes BMI value into weight categories
   @param fBMI is the Body Mass Index value
   @return the BMI category (UNDERWEIGHT, NORMAL, OVERWEIGHT, or OBESE)
*/
int
categorizeBMI(float fBMI)
{
    if (fBMI < 18.5)
        return UNDERWEIGHT;
    else if (fBMI < 25.0)
        return NORMAL;
    else if (fBMI < 30.0)
        return OVERWEIGHT;
    else
        return OBESE;
}

/* Calculates ideal weight based on height and gender
   Precondition: nHeightFt and nHeightIn are valid height measurements
   @param nHeightFt is the height in feet
   @param nHeightIn is the height in inches
   @param cGender is the gender ('m' for male, 'f' for female)
   @return the calculated ideal weight in pounds
*/
float
calculateIdealWeight(int nHeightFt, int nHeightIn, char cGender)
{
    int nDiffBelow, nDiffAbove;
    float fIdealWeight = 0.0;

    /* Basic input validation */
    if (nHeightFt < 0 || nHeightIn < 0 || (nHeightFt == 0 && nHeightIn == 0))
        return 0;

	/* Computes the surplus and shortage of height from/to five feet */
    nDiffBelow = FIVE_FEET - (FTtoInches(nHeightFt) + nHeightIn);
    nDiffAbove = (FTtoInches(nHeightFt) + nHeightIn) - FIVE_FEET;

    switch (cGender)
    {
        case MALE:
            if (nHeightFt < 5)
                fIdealWeight = 105 - 5 * nDiffBelow;
            else
                fIdealWeight = 105 + 5 * nDiffAbove;
            break;

        case FEMALE:
            if (nHeightFt < 5)
                fIdealWeight = 95 - 4 * nDiffBelow;
            else
                fIdealWeight = 95 + 4 * nDiffAbove;
            break;

        default:
            return 0;
    }

    /* Ensure ideal weight is not negative */
    if (fIdealWeight < 0)
        return 0;

    return fIdealWeight;
}

/* Sorts activities array by MET values in descending order using bubble sort
   Only considers activities selected by the user
   Precondition: arrActivities is properly initialized
   @param arrActivities is the array of activities to sort
   @param nSize is the size of the array
*/
void
sortActsByMET(structActivityType arrActivities[], int nSize)
{
    int nOuter, nInner;
    structActivityType stTemp;

    for (nOuter = 0; nOuter < nSize - 1; nOuter++)
    {
        for (nInner = 0; nInner < nSize - nOuter - 1; nInner++)
        {
            if (arrActivities[nInner].fMET < arrActivities[nInner + 1].fMET)
            {
                stTemp = arrActivities[nInner];
                arrActivities[nInner] = arrActivities[nInner + 1];
                arrActivities[nInner + 1] = stTemp;
            }
        }
    }
}

/* Allocates exercise minutes to achieve ideal weight safely
   Precondition: arrActivities contains valid activities, 
                 fWeightLbs > fIdealWeight
   @param arrActivities is the array of available activities
   @param fWeightLbs is the current weight in pounds
   @param fIdealWeight is the target ideal weight in pounds
   @param nFirstActMinutes is a pointer to store minutes for first activity
   @param fFirstActCalBurned is a pointer to store calories burned for first activity
   @param nSecondActIndex is a pointer to store index of second activity
   @param fSecondActMinutes is a pointer to store minutes for second activity
   @param fSecondActCalBurned is a pointer to store calories burned for second activity
   @param nSize is the size of the activities array
*/
void
allocateMinutesPerExercise(structActivityType arrActivities[], 
                          float fWeightLbs, float fIdealWeight,
                          int *nFirstActMinutes, float *fFirstActCalBurned, 
                          int *nSecondActIndex, float *fSecondActMinutes, 
                          float *fSecondActCalBurned, int nSize)
{
    float fWeightToLose = fWeightLbs - fIdealWeight;
    float fCaloriesToBurn = fWeightToLose * LBS_TO_CAL;
    float fWeightKg = LbsToKG(fWeightLbs);
    float fDailyCalorieTarget;
    int nActIndex, nMinutes;
    structActivityType arrSelectedActs[MAX_ACTIVITIES];
    int nSelectedCount = 0;
    int i;
    
   
    /* Filter only selected activities */
    for (i = 0; i < nSize; i++)
    {
        if (arrActivities[i].bSelected && arrActivities[i].sName[0] != '\0')
        {
            arrSelectedActs[nSelectedCount] = arrActivities[i];
            nSelectedCount++;
        }
    }
    
    /* If no activities selected, use all activities */
    if (nSelectedCount == 0)
    {
        printf("Please select an activity in settings.\n");
    }
    else
    {
    	sortActsByMET(arrSelectedActs, nSelectedCount);
	    
	    /* Limit to 2 pounds per week (7000 calories per week) */
	    if (fCaloriesToBurn > 7000) 
	    { 
	        fCaloriesToBurn = 7000; 
	    }
	    
	    fDailyCalorieTarget = fCaloriesToBurn / 7.0;
	    
	    /* Initialize return values */
	    *nSecondActIndex = -1;
	    *fFirstActCalBurned = 0;
	    *fSecondActCalBurned = 0;
	    
	    /* Start with maximum time on highest MET activity */
	    *nFirstActMinutes = MAX_MINUTES_PER_DAY;
	    *fFirstActCalBurned = calculateCaloriesBurned(arrSelectedActs[0].fMET, 
	                                                  fWeightKg, *nFirstActMinutes);
	    
	    /* Reduce first activity time if it exceeds daily target */
	    while (*fFirstActCalBurned > fDailyCalorieTarget && *nFirstActMinutes > 0)
	    {
	        (*nFirstActMinutes)--;
	        *fFirstActCalBurned = calculateCaloriesBurned(arrSelectedActs[0].fMET, 
	                                                      fWeightKg, *nFirstActMinutes);
	    }
	    
	    /* Try to find best second activity */
	    while (*nSecondActIndex == -1 && *nFirstActMinutes > 0)
	    {
	        float fCaloriesRemaining = fDailyCalorieTarget - *fFirstActCalBurned;
	        float fHighestCalBurned = 0;
	        int nMaxAvailableMinutes = MAX_MINUTES_PER_DAY - *nFirstActMinutes;
	        
	        /* Search through selected activities for best fit */
	        for (nActIndex = 1; nActIndex < nSelectedCount; nActIndex++)
	        {
	            /* Try all possible minute allocations for this activity */
	            for (nMinutes = 1; nMinutes <= nMaxAvailableMinutes; nMinutes++)
	            {
	                float fCalBurned = calculateCaloriesBurned(
	                    arrSelectedActs[nActIndex].fMET, fWeightKg, nMinutes);
	                
	                /* Keep track of highest calories that fits within remaining budget */
	                if (fCalBurned <= fCaloriesRemaining && fCalBurned > fHighestCalBurned)
	                {
	                    fHighestCalBurned = fCalBurned;
	                    *nSecondActIndex = nActIndex;
	                    *fSecondActMinutes = nMinutes;
	                }
	            }
	        }
	        
	        /* If no valid second activity found, reduce first activity and retry */
	        if (*nSecondActIndex == -1 && *nFirstActMinutes > 0)
	        {
	            (*nFirstActMinutes)--;
	            *fFirstActCalBurned = calculateCaloriesBurned(arrSelectedActs[0].fMET, 
	                                                          fWeightKg, *nFirstActMinutes);
	        }
	    }
	    
	    /* Calculate final second activity calories if found */
	    if (*nSecondActIndex != -1)
	    {
	        *fSecondActCalBurned = calculateCaloriesBurned(
	            arrSelectedActs[*nSecondActIndex].fMET, fWeightKg, *fSecondActMinutes);
	    }
	    
	    /* Copy selected activities back to original array for display */
	    for (i = 0; i < nSelectedCount && i < nSize; i++)
	    {
	        arrActivities[i] = arrSelectedActs[i];
	    }
	}
    
}

/* Displays the appropriate results based on BMI category
   Precondition: arrActivities is properly initialized, fWeightLbs > 0
   @param arrActivities is the array of available activities
   @param fWeightLbs is the current weight in pounds
   @param fIdealWeight is the calculated ideal weight in pounds
   @param nCategory is the BMI category
*/
void
displayResults(structActivityType arrActivities[], float fWeightLbs, 
               float fIdealWeight, int nCategory)
{
    structActivityType arrUnderweightActs[30] = 
    {
        {"Brisk Walking", 4.3},
        {"Swimming", 7.0},
        {"Golf", 4.8},
        {"Weight Lifting", 6.0},
    };
    
    int nFirstActMinutes = 0;
    int nSecondActIndex = 0;
    float fFirstActCalBurned, fSecondActCalBurned, fSecondActMinutes;
    float fWeightToLose = fWeightLbs - fIdealWeight;
    float fTotalCaloriesToBurn = fWeightToLose * LBS_TO_CAL;
    float fTotalWeeks;
    
    switch (nCategory)
    {
        case NORMAL:
            printf("\nKeep up what you're doing. ");
            printf("Don't forget to eat healthy foods while exercising!\n");
            break;
            
        case UNDERWEIGHT:
            printf("Your ideal weight is: %.2f lbs\n", fIdealWeight);
            recommendProgramUnderweight(arrUnderweightActs, 4);
            break;
            
        case OVERWEIGHT:
        case OBESE:
        
            allocateMinutesPerExercise(arrActivities, fWeightLbs, fIdealWeight, 
                                      &nFirstActMinutes, &fFirstActCalBurned,
                                      &nSecondActIndex, &fSecondActMinutes, 
                                      &fSecondActCalBurned, 12);
                                      
            displaySubHeader(EXERCISE_PROGRAM);
            
            printf("Your ideal weight is: %.2f lbs\n", fIdealWeight);
            printf("Weight to lose: %.2f lbs\n", fWeightToLose);
            printf("Total calories to burn: %.2f\n\n", fTotalCaloriesToBurn);
            
            printf("[1] %s (%.2f MET)\n", arrActivities[0].sName, 
                   arrActivities[0].fMET);
            printf("    Minutes per day: %d\n", nFirstActMinutes);
            printf("    Calories burned: %.2f\n\n", fFirstActCalBurned);
            
            printf("[2] %s (%.2f MET)\n", 
                   arrActivities[nSecondActIndex].sName, 
                   arrActivities[nSecondActIndex].fMET);
            printf("    Minutes per day: %.0f\n", fSecondActMinutes);
            printf("    Calories burned: %.2f\n\n", fSecondActCalBurned);
            
            fTotalWeeks = fTotalCaloriesToBurn / 
                         ((fFirstActCalBurned + fSecondActCalBurned) * 7.0);
            printf("Total program duration: %.2f weeks (or %.1f days)\n", fTotalWeeks, fTotalWeeks*7);
            printf("Total daily minutes: %.0f\n", 
                   nFirstActMinutes + fSecondActMinutes);
            printf("Total daily calories burned: %.2f\n", 
                   fFirstActCalBurned + fSecondActCalBurned);
            
            break;
    }
}

/* Recommends exercise program for underweight individuals
   Precondition: arrUnderweightActs contains at least nNumActs activities
   @param arrUnderweightActs is the array of suitable activities
   @param nNumActs is the number of activities in the array
*/
void 
recommendProgramUnderweight(structActivityType arrUnderweightActs[], int nNumActs)
{
    int nActCount;
    int usedIndices[3] = {-1, -1, -1};
    int isUsed, randIndex, nCtr;

    displaySubHeader(EXERCISE_PROGRAM);

    for (nActCount = 0; nActCount < 3; nActCount++)
    {
        do
        {
            randIndex = rand() % nNumActs;

            /* Check if this index has already been used */
            isUsed = 0;
            for (nCtr = 0; nCtr < nActCount && !(isUsed); nCtr++)
            {
                if (usedIndices[nCtr] == randIndex)
                {
                    isUsed = 1;
                }
            }
        } while (isUsed);

        usedIndices[nActCount] = randIndex;
        printf("[%d] %s (%.1f MET)\n", nActCount + 1, 
               arrUnderweightActs[randIndex].sName, 
               arrUnderweightActs[randIndex].fMET);
    }

    printf("\nMinimum recommended time per day: 30 minutes.\n");
}

/* Adds a new activity to the activities array
   Precondition: arrActivities has space for additional activities
   @param arrActivities is the array to add the activity to
   @param nNumActivities is a pointer to the current number of activities
*/
void 
addActivity(structActivityType arrActivities[], int *nNumActivities) 
{
    char sTempName[30];
    float fTempMET;
    
    if (*nNumActivities >= MAX_ACTIVITIES) 
    {
        printf("Cannot add more activities. Maximum reached.\n");
    }
    else
    {
        printf("Enter the name of the exercise: ");
        scanf(" %[^\n]", sTempName);
    
        printf("Enter the MET value of %s: ", sTempName);
        scanf("%f", &fTempMET);
    
        strcpy(arrActivities[*nNumActivities].sName, sTempName);
        arrActivities[*nNumActivities].fMET = fTempMET;
        arrActivities[*nNumActivities].bSelected = 1; /* Auto-select new activity */
    
        (*nNumActivities)++;
    
        printf("Activity '%s' with MET %.2f added successfully!\n", 
               sTempName, fTempMET);
    } 
}

/* Removes an activity from the activities array
   Precondition: arrActivities contains at least one activity
   @param arrActivities is the array to remove the activity from
   @param nNumActivities is a pointer to the current number of activities
*/
void 
removeActivity(structActivityType arrActivities[], int *nNumActivities) 
{
    int nRemoveIndex, i;
    
    if (*nNumActivities == 0) 
    {
        printf("No activities to remove.\n");
    }
    else
    {
        printf("Enter the number of the activity to remove: ");
        scanf("%d", &nRemoveIndex);
    
        if (nRemoveIndex < 1 || nRemoveIndex > *nNumActivities) 
        {
            printf("Invalid activity number.\n");
        }
        else
        {
            /* Convert to 0-based index */
            nRemoveIndex--;
        
            /* Shift remaining activities left */
            for (i = nRemoveIndex; i < *nNumActivities - 1; i++) 
            {
                arrActivities[i] = arrActivities[i + 1];
            }
        
            (*nNumActivities)--;
        
            printf("Activity removed successfully.\n");
        }
    }
}

/* Allows user to select preferred activities for exercise program
   Precondition: arrActivities is properly initialized with at least one activity
   @param arrActivities is the array of available activities
   @param nNumActivities is the current number of activities
*/
void 
selectActivities(structActivityType arrActivities[], int nNumActivities)
{
    int nChoice, i;
    int bContinueSelecting;
    int nDispNum = 0;
    
    if (nNumActivities == 0)
    {
        printf("\nNo activities available. Please add activities first.\n");
    }
    else
    {
        printf("\n");
        displayLineSeparator(34, EQUAL_SIGN);
        printf("    Select Preferred Activities\n");
        displayLineSeparator(34, EQUAL_SIGN);
        
        printf("\nCurrent selection:\n");
        
        /* Display current selections */
        for (i = 0; i < nNumActivities; i++)
        {
            if (arrActivities[i].sName[0] != '\0')
            {
            	nDispNum++;
                printf("  [%d] %s - %s\n", nDispNum, arrActivities[i].sName,
                       arrActivities[i].bSelected ? "SELECTED" : "Not Selected");
            }
        }
        
        bContinueSelecting = 1;
        
        while (bContinueSelecting)
        {
            printf("\nEnter activity number to toggle selection (0 to finish): ");
            scanf("%d", &nChoice);
            
            if (nChoice == 0)
            {
                bContinueSelecting = 0;
            }
            else if (nChoice < 1 || nChoice > nNumActivities)
            {
                printf("Invalid activity number. Please try again.\n");
            }
            else
            {
                /* Toggle selection */
                arrActivities[nChoice - 1].bSelected = 
                    !arrActivities[nChoice - 1].bSelected;
                
                printf("Activity '%s' is now %s.\n", 
                       arrActivities[nChoice - 1].sName,
                       arrActivities[nChoice - 1].bSelected ? "SELECTED" : "UNSELECTED");
            }
        }
        
        /* Display final selections */
        printf("\n");
        displayLineSeparator(34, DASH);
        printf("Final selections:\n");
        
        for (i = 0; i < nNumActivities; i++)
        {
            if (arrActivities[i].sName[0] != '\0' && arrActivities[i].bSelected)
            {
                printf("  - %s (%.1f MET)\n", arrActivities[i].sName, 
                       arrActivities[i].fMET);
            }
        }
        
        displayLineSeparator(34, DASH);
    }
}

/* Main program entry point
   @return 0 on successful execution
*/
int
main()
{
    int nSize = 30;
    int nNumActivities = 12;
    int nMenuOption;
    char cGender;
    float fWeight, fBMI, fIdealWeight;
    int nHeightFt, nHeightIn;
    int nCategory;
    
    structActivityType arrActivities[30] = 
    {
        {"Brisk Walking", 4.3, 1},
        {"Shopping / Malling", 2.3, 1},
        {"Swimming", 7.0, 1},
        {"Leisure Biking", 6.8, 1},
        {"Running", 12.3, 1},
        {"Dance Revo", 7.2, 1},
        {"Zumba", 7.8, 1},
        {"Yoga", 2.3, 1},
        {"Golf", 4.8, 1},
        {"Basketball", 6.5, 1},
        {"Weight Lifting", 6.0, 1},
        {"Wii Fit", 3.8, 1}    
    };
    
    srand(time(NULL));
    
    displayHeader();
    
    do
    {
        nMenuOption = askMenuOptions(MAIN);    
        validateMenuOption(nMenuOption, MAIN);
        
        switch(nMenuOption)
        {
            case START_RECO:
                displayLineSeparator(34, DASH); 
                
                getUserData(&cGender, &fWeight, &nHeightFt, &nHeightIn);
                
                displayLineSeparator(34, DASH);
                
                fBMI = computeBMI(fWeight, nHeightFt, nHeightIn);
                nCategory = categorizeBMI(fBMI);
                fIdealWeight = calculateIdealWeight(nHeightFt, nHeightIn, cGender);
                
                displayResults(arrActivities, fWeight, fIdealWeight, nCategory);    
                break;
                
            case SETTINGS:
                do
                {
                    nMenuOption = askMenuOptions(SETTINGS);
                    validateMenuOption(nMenuOption, SETTINGS);
                    
                    switch (nMenuOption)
                    {
                        case SEE_EXERCISE_LIST:
                            displayExerciseList(arrActivities, nSize);
                            break;
                        
                        case ADD_EXERCISE:
                            displayExerciseList(arrActivities, nSize);
                            addActivity(arrActivities, &nNumActivities);
                            break;
                        
                        case REMOVE_EXERCISE:
                            displayExerciseList(arrActivities, nSize);
                            removeActivity(arrActivities, &nNumActivities);
                            break;
                        
                        case SELECT_ACTIVITIES:
                            selectActivities(arrActivities, nNumActivities);
                            break;
                    } 
                } while (nMenuOption != EXIT_SETTINGS);
                break;
        }
    } while (nMenuOption != EXIT);
    
    printf("\nThank you for using Make Me Fit!\n");
    printf("Stay healthy and keep exercising!\n\n");
    
    return 0;
}
