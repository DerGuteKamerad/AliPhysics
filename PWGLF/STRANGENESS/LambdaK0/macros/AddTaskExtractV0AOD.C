AliAnalysisTaskExtractV0AOD *AddTaskExtractV0AOD( Bool_t lSwitchIsNuclear     = kFALSE, 
                                                  Bool_t lSwitchIsLowEnergyPP = kFALSE,
                                                  Bool_t lSwitchUseOnTheFly   = kFALSE, 
                                                  TString lTriggerMask = "kMB",
                                                  const TString lMasterJobSessionFlag = "")
{
// Creates, configures and attaches to the train a cascades check task.
   // Get the pointer to the existing analysis manager via the static access method.
   //==============================================================================
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) {
      ::Error("AddTaskExtractV0", "No analysis manager to connect to.");
      return NULL;
   }   

   // Check the analysis type using the event handlers connected to the analysis manager.
   //==============================================================================
   if (!mgr->GetInputEventHandler()) {
      ::Error("AddTaskExtractV0", "This task requires an input event handler");
      return NULL;
   }   
   TString type = mgr->GetInputEventHandler()->GetDataType(); // can be "ESD" or "AOD"

   // Create and configure the task
	 AliAnalysisTaskExtractV0AOD *taskv0extract = new AliAnalysisTaskExtractV0AOD("taskv0extract");

   //Configuration
   taskv0extract -> SetIsNuclear     ( lSwitchIsNuclear     );
   taskv0extract -> SetIsLowEnergyPP ( lSwitchIsLowEnergyPP );
   taskv0extract -> SetUseOnTheFly   ( lSwitchUseOnTheFly   );
   taskv0extract -> SetTriggerMask   ( lTriggerMask         );


   mgr->AddTask(taskv0extract);

   TString outputFileName = AliAnalysisManager::GetCommonFileName();
   
   outputFileName += ":PWG2CheckLambda";
   //if (lCollidingSystems) outputFileName += "_AA_";
   outputFileName += "_PP";
   if (mgr->GetMCtruthEventHandler()) outputFileName += "_MC";
   //if(lMasterJobSessionFlag.Length()) outputFileName += lMasterJobSessionFlag.Data();
   
   Printf("Set OutputFileName : \n %s\n", outputFileName.Data() );

   AliAnalysisDataContainer *coutputList = mgr->CreateContainer("clistV0",
							     TList::Class(),
							     AliAnalysisManager::kOutputContainer,
							     outputFileName );
   AliAnalysisDataContainer *coutputTree = mgr->CreateContainer("cTree",
							     TTree::Class(),
							     AliAnalysisManager::kOutputContainer,
							     outputFileName );
   
   //This one you should merge in file-resident ways...
   coutputTree->SetSpecialOutput();

   mgr->ConnectInput( taskv0extract, 0, mgr->GetCommonInputContainer());
   mgr->ConnectOutput(taskv0extract, 1, coutputList);
   mgr->ConnectOutput(taskv0extract, 2, coutputTree);
   
   return taskv0extract;
}   
