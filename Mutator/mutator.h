#include "strList.h"
#include "strListv2.h"
#include "rgxList.h"
#include "sumBox.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <Windows.h>
#include <regex>
#include <iterator>
#include <dirent.h>
#include <algorithm>
#include <vector>
#include <list>
#include <sstream> 
#include <experimental/filesystem>
#include <msclr/marshal_cppstd.h>
#include <chrono>
using namespace std;
namespace fs = std::experimental::filesystem;
using namespace System;
using namespace System::Windows::Forms;

#pragma once

std::string converted_pathFrm, converted_pathTo, converted_currItem;
bool has_suffix(const string& s, const string& suffix);
int putInList(string dPath, list <string> &dList);
int findInFile(string readout, string dFile, string searchStr, string blck, rgxList regxs);
//int mutateFiles(string filePathFrm, string filePathTo, string currMut);
long long mutateFiles(string filePathFrm, string filePathTo, string currMut);
string intToString(int intChange);
long long timeTaken = 0;

namespace Mutator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnMutate;
	private: System::Windows::Forms::Button^  btnBrowse;
	private: System::Windows::Forms::Label^  lblCreator;
	private: System::Windows::Forms::Label^  lblBrowse;

	private: System::Windows::Forms::Label^  lblMutants;
	private: System::Windows::Forms::Label^  lblPathto;
	private: System::Windows::Forms::Button^  btnPathto;
	private: System::Windows::Forms::CheckedListBox^  lstBoxMutants;
	private: System::Windows::Forms::Label^  lblTime;



	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnMutate = (gcnew System::Windows::Forms::Button());
			this->btnBrowse = (gcnew System::Windows::Forms::Button());
			this->lblCreator = (gcnew System::Windows::Forms::Label());
			this->lblBrowse = (gcnew System::Windows::Forms::Label());
			this->lblMutants = (gcnew System::Windows::Forms::Label());
			this->lblPathto = (gcnew System::Windows::Forms::Label());
			this->btnPathto = (gcnew System::Windows::Forms::Button());
			this->lstBoxMutants = (gcnew System::Windows::Forms::CheckedListBox());
			this->lblTime = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btnMutate
			// 
			this->btnMutate->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->btnMutate->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnMutate->Location = System::Drawing::Point(12, 153);
			this->btnMutate->Name = L"btnMutate";
			this->btnMutate->Size = System::Drawing::Size(190, 59);
			this->btnMutate->TabIndex = 3;
			this->btnMutate->Text = L"Mutate!";
			this->btnMutate->UseVisualStyleBackColor = true;
			this->btnMutate->Click += gcnew System::EventHandler(this, &MyForm::btnMutate_Click);
			// 
			// btnBrowse
			// 
			this->btnBrowse->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnBrowse->Location = System::Drawing::Point(260, 10);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(75, 23);
			this->btnBrowse->TabIndex = 0;
			this->btnBrowse->Text = L"Browse...";
			this->btnBrowse->UseVisualStyleBackColor = true;
			this->btnBrowse->Click += gcnew System::EventHandler(this, &MyForm::btnBrowse_Click);
			// 
			// lblCreator
			// 
			this->lblCreator->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->lblCreator->AutoSize = true;
			this->lblCreator->Location = System::Drawing::Point(315, 202);
			this->lblCreator->Name = L"lblCreator";
			this->lblCreator->Size = System::Drawing::Size(147, 13);
			this->lblCreator->TabIndex = 2;
			this->lblCreator->Text = L"Made by Henrik Runge, 2017";
			// 
			// lblBrowse
			// 
			this->lblBrowse->AutoSize = true;
			this->lblBrowse->Location = System::Drawing::Point(13, 15);
			this->lblBrowse->Name = L"lblBrowse";
			this->lblBrowse->Size = System::Drawing::Size(142, 13);
			this->lblBrowse->TabIndex = 3;
			this->lblBrowse->Text = L"Pick project folder to mutate:";
			// 
			// lblMutants
			// 
			this->lblMutants->AutoSize = true;
			this->lblMutants->Location = System::Drawing::Point(13, 65);
			this->lblMutants->Name = L"lblMutants";
			this->lblMutants->Size = System::Drawing::Size(200, 13);
			this->lblMutants->TabIndex = 5;
			this->lblMutants->Text = L"Select mutant(s) to apply on folder (WIP):";
			// 
			// lblPathto
			// 
			this->lblPathto->AutoSize = true;
			this->lblPathto->Location = System::Drawing::Point(13, 115);
			this->lblPathto->Name = L"lblPathto";
			this->lblPathto->Size = System::Drawing::Size(199, 13);
			this->lblPathto->TabIndex = 6;
			this->lblPathto->Text = L"Select where you want the new folder(s):";
			// 
			// btnPathto
			// 
			this->btnPathto->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnPathto->Location = System::Drawing::Point(260, 110);
			this->btnPathto->Name = L"btnPathto";
			this->btnPathto->Size = System::Drawing::Size(75, 23);
			this->btnPathto->TabIndex = 2;
			this->btnPathto->Text = L"Browse...";
			this->btnPathto->UseVisualStyleBackColor = true;
			this->btnPathto->Click += gcnew System::EventHandler(this, &MyForm::btnPathto_Click);
			// 
			// lstBoxMutants
			// 
			this->lstBoxMutants->FormattingEnabled = true;
			this->lstBoxMutants->Items->AddRange(gcnew cli::array< System::Object^  >(9) {
				L"Gain", L"Sum", L"Sum2Product", L"Product2Sum",
					L"Remove Abs", L"GotoFrom", L"Sum2Add", L"Sum2Subtract", L"RelationalOperator"
			});
			this->lstBoxMutants->Location = System::Drawing::Point(260, 39);
			this->lstBoxMutants->Name = L"lstBoxMutants";
			this->lstBoxMutants->Size = System::Drawing::Size(202, 64);
			this->lstBoxMutants->TabIndex = 7;
			// 
			// lblTime
			// 
			this->lblTime->AutoSize = true;
			this->lblTime->Location = System::Drawing::Point(260, 153);
			this->lblTime->Name = L"lblTime";
			this->lblTime->Size = System::Drawing::Size(51, 13);
			this->lblTime->TabIndex = 8;
			this->lblTime->Text = L"Last run: ";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(474, 224);
			this->Controls->Add(this->lblTime);
			this->Controls->Add(this->lstBoxMutants);
			this->Controls->Add(this->btnPathto);
			this->Controls->Add(this->lblPathto);
			this->Controls->Add(this->lblMutants);
			this->Controls->Add(this->lblBrowse);
			this->Controls->Add(this->lblCreator);
			this->Controls->Add(this->btnBrowse);
			this->Controls->Add(this->btnMutate);
			this->MaximizeBox = false;
			this->Name = L"MyForm";
			this->ShowIcon = false;
			this->Text = L"Mutator.exe";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btnMutate_Click(System::Object^  sender, System::EventArgs^  e) {

		int useNum;
		string endNum;

		for (int i = 0; i < lstBoxMutants->CheckedItems->Count; i++) {
			System::String^ currItem = lstBoxMutants->CheckedItems[i]->ToString();
			converted_currItem = msclr::interop::marshal_as< std::string >(currItem);
			useNum = i+1;
			endNum = intToString(useNum);
			timeTaken = mutateFiles(converted_pathFrm, (converted_pathTo + "\\" + converted_currItem), converted_currItem);
		}
		lblTime->Text = "Last run: " + timeTaken + " ms.";
	}

	private: System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
		FolderBrowserDialog^ folderBrowserDialog1;
		folderBrowserDialog1 = gcnew System::Windows::Forms::FolderBrowserDialog;

		// Show the FolderBrowserDialog.
		System::Windows::Forms::DialogResult result = folderBrowserDialog1->ShowDialog();
		if (result == System::Windows::Forms::DialogResult::OK)
		{
			System::String^ folderName = folderBrowserDialog1->SelectedPath;
			//lePath = folderName;
			converted_pathFrm = msclr::interop::marshal_as< std::string >(folderName);
			//System::String^
			//
			//
		}
	}
private: System::Void btnPathto_Click(System::Object^  sender, System::EventArgs^  e) {
	FolderBrowserDialog^ folderBrowserDialog2;
	folderBrowserDialog2 = gcnew System::Windows::Forms::FolderBrowserDialog;

	// Show the FolderBrowserDialog.
	System::Windows::Forms::DialogResult result = folderBrowserDialog2->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		System::String^ folderName = folderBrowserDialog2->SelectedPath;
		converted_pathTo = msclr::interop::marshal_as< std::string >(folderName);
		//System::String^
		//
		//
	}
}
};
}
