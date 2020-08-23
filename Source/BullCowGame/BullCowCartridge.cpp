// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include <iostream>

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
   
    const FString WordListPath = FPaths::ProjectContentDir()/TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(WordList, *WordListPath);
    
    

    FBullCowCount Count;
    bLvlSet=false;
    bGameOver=true;
    PrintIntro();
   // SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{   
   
    
     if(!bLvlSet&&bGameOver)
     {
        
        Isograms=GetValidWords(WordList,GetDifficulty(Input));
         
         bLvlSet=true;
         //bGameOver=true;
         
     }
        if(bGameOver&&bLvlSet )
        {
         // std::cin>>Level;
        // ClearScreen();
      
          SetupGame();
          bGameOver=false;
          return;
         
        }
        if(!bGameOver&& bLvlSet) //Checking player Guess
        {
          ProcessGuess(Input);
          return;
        }  
  }

    //ask if want to play again
    //check input
    //play again if yes quit if no
void UBullCowCartridge::PrintIntro()
{
  PrintLine(TEXT("\n\nWelcome to Bulls and Cows")) ;
PrintLine(TEXT("To set difficulty press: \na=easy\nb=normal\nc=hard\nd=hardcore")) ;
}

void UBullCowCartridge::SetupGame()
{
  
  //set word
  HiddenWord=Isograms[FMath::RandRange(0,Isograms.Num()-1)].ToLower();
  Lives=HiddenWord.Len()*2;
  
//PrintLine(TEXT("HiddenWord is : %s"),*HiddenWord);
 //Welcome msg
  
  PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len()) ;
  PrintLine(TEXT("You have only %i chances"), Lives) ;
  
}


void UBullCowCartridge::EndGame()
{
  bGameOver=true;
  bLvlSet=false;
PrintLine(TEXT("HiddenWord was : %s"),*HiddenWord);

 PrintIntro();
  
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
  if (HiddenWord == Guess)
  { 
    PrintLine(TEXT("You Win")) ;
    EndGame();
    return;
  }
  
  if(HiddenWord.Len()!=Guess.Len())
  {
    PrintLine(TEXT("Wrong word lenght!Try Again"));
    return;
  }

  if(!IsIsogram(Guess))
  {
     PrintLine(TEXT("No repeating letters! Try Again")) ;
     return;
  } 

   PrintLine(TEXT("Wrong guess\nYou have %i chances left"),--Lives) ;
            
  if(Lives<=0)
  {
    EndGame();
    return;
  }
  FBullCowCount Score= GetBullCows(Guess);
  PrintLine(TEXT("You have %i Bulls and %i Cows"),Score.Bulls,Score.Cows) ;
}   

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
  for(int32 CharNum=0; CharNum<Word.Len(); CharNum++)
  {
    for(int32 Index=CharNum+1; Index<Word.Len(); Index++)
    {
      if(Word[CharNum]==Word[Index])
      {
        return false;
      }
    }
  }
  return true;
} 
 
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& ListOfWords,int32 Difficulty) const
{
  
  TArray<FString> ValidWords;
  for(FString CurrentWord : ListOfWords )
  {
    if(CurrentWord.Len()<=Difficulty && CurrentWord.Len()>=Difficulty && IsIsogram(CurrentWord))
    {
   
      
      ValidWords.Emplace(CurrentWord);
      
    //  PrintLine(TEXT(" word is %s "), *ValidWords.Last());   
    }         
  }
  return ValidWords;
}


FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess)const
{
  FBullCowCount Count;

  for(int32 GuessIndex=0; GuessIndex<Guess.Len(); GuessIndex++)
  {
    if(Guess[GuessIndex]==HiddenWord[GuessIndex])
    {
      Count.Bulls++;
      continue;
    }
    for(int32 HiddenIndex=0;HiddenIndex<HiddenWord.Len();HiddenIndex++)
    {
      if(Guess[GuessIndex]==HiddenWord[HiddenIndex])
      {
        Count.Cows++;
        break;
      }
    }
    
    
    

  }
return Count;
}
int32 UBullCowCartridge::GetDifficulty(const FString& DiffInput)
{
 
FString Lvla=TEXT("a"),Lvlb=TEXT("b"),Lvlc=TEXT("c"),Lvld=TEXT("d");

if(DiffInput==Lvla)
{
  return 4;
}
if(DiffInput==Lvlb)
{
  return 5;
}
if(DiffInput==Lvlc)
{
  return 6;
}
if(DiffInput==Lvld)
{
  return 7;
}
return 3;
}