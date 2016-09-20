#include <fstream.h>
#include <iostream.h>
#include "math.h"
#include <vector>
#include <string>


#include <sstream>

std::string IntToStr(int i)
{
std::stringstream out;
out << i;
return out.str();
//possible alternativ to char*		//	sprintf(VariableName[i],"%d\n", i);

}



int compare (std::string s1, std::string s2)
{
	
	if (s1.substr(0,s1.find(' '))==s2.substr(0,s2.find(' '))) return 1;
	else return 0;

}

int compare (char &st1, std::string s2)
{
	std::string s1;
	s1=st1;
	return compare(s1,s2);

}




class Variable
{
public:
	int size1;
	int size2;
	int type;
	int *IntData;
	bool *boolData;
	double *doubleData;
	std::vector<std::string> CharData;
public:

Variable ()
{
}
	
Variable (int Psize1,int Psize2,int Ptype)
{
	size1=Psize1;
	size2=Psize2;
	type=Ptype;
	if (type==1) IntData=new int [size1];
	if (type==2) doubleData=new double [size1];
	if (type==3) CharData.resize(0);
	if (type==4) boolData=new bool [size1];
}
};

Variable FindVariable(ifstream inf, std::string Section, std::string VariableName)
	{
		char st1[80];
		int flag1, flag2;
		do
		{
			if (!(inf.getline(st1,80))) 
			{
				cout<<"Cannot find variable "<<Section.c_str()<<"  "<<VariableName.c_str()<<endl;
				exit(1);
			}

			flag1=compare(st1,Section);
			if (flag1==1) 
				
			{
				inf.getline(st1,80);
				flag2=compare(st1,VariableName);	
			}				
		}
		while ((flag1!=1)||(flag2!=1));



		int size1,size2, type;
		inf>>size1>>size2>>type;

		Variable Var(size1,size2,type);
		if (size1!=size2) cout<<"Warning size1!=size2";
		if ((type>4)||(type<1)) cout<<"Unknown type of data";
		
		if (type==1)
			for (int i=0; i<size1; i++)
				inf>>Var.IntData[i];
		if (type==2)
			for (int i=0; i<size1; i++)
				inf>>Var.doubleData[i];
		if (type==3)
		{
			int N;
			N=size1/160;
			size1=N;
									
			char ChDa[160];
			for (int i=0; i<N; i++)
			{
				inf>>ChDa;
				Var.CharData.push_back(ChDa);
			}
		}
		if (type==4)
			for (int i=0; i<size1; i++)
			{
				char cc;
				inf>>cc;
				if (cc=='T') Var.boolData[i]=true;
				else Var.boolData[i]=false;
			}
		return Var;
	}	


void FindVariable(ifstream inf, std::string Section, std::string VariableName, double *OutputData)
	{
		char st1[80];
		int flag1, flag2;
		do
		{
			if (!(inf.getline(st1,80))) 
			{
				cout<<"Cannot find variable "<<Section.c_str()<<"  "<<VariableName.c_str()<<endl;
				exit(1);
			}

			flag1=compare(st1,Section);
			if (flag1==1) 
				
			{
				inf.getline(st1,80);
				flag2=compare(st1,VariableName);	
			}				
		}
		while ((flag1!=1)||(flag2!=1));




		int size1,size2, type;
		inf>>size1>>size2>>type;

		for (int i=0; i<size1; i++)
			inf>>OutputData[i];


	}	





class Grid
{
public:
	bool Unrestricted;
	int nr_points_x, nr_points_y, nr_points_z;
	double *Start_point, *x_vector, *y_vector, *z_vector;
	int nr_symm;
	std::vector<std::string> labels;
	double *Pot;

	
	Grid()
	{
	}
	void Grid1(ifstream inf )
	{
		std::string Section, VariableName;
		Section="Grid";
		
		Variable Var1;

		VariableName="Start_point";
		Var1=FindVariable(inf, Section, VariableName);
		Start_point=Var1.doubleData;
		
		
		VariableName="nr of points x";
		
		Var1=FindVariable(inf, Section, VariableName);
		nr_points_x=Var1.IntData[0];
		VariableName="nr of points y";
		Var1=FindVariable(inf, Section, VariableName);
		nr_points_y=Var1.IntData[0];
		VariableName="nr of points z";
		Var1=FindVariable(inf, Section, VariableName);
		nr_points_z=Var1.IntData[0];
		cout<<"Nx,Ny,Nz="<<nr_points_x<<"  "<<nr_points_y<<"  "<<nr_points_z<<"\n";

		VariableName="x-vector                        ";
		Var1=FindVariable(inf, Section, VariableName);
		x_vector=Var1.doubleData;
		VariableName="y-vector                        ";
		Var1=FindVariable(inf, Section, VariableName);
		y_vector=Var1.doubleData;
		VariableName="z-vector                        ";
		Var1=FindVariable(inf, Section, VariableName);
		z_vector=Var1.doubleData;
		
		cout<<"Start_point"<<Start_point[0]<<"  "<<Start_point[1]<<"  "<<Start_point[2]<<"\n";
		cout<<"x-vector"<<x_vector[0]<<"  "<<x_vector[1]<<"  "<<x_vector[2]<<"\n";
		cout<<"y-vector"<<y_vector[0]<<"  "<<y_vector[1]<<"  "<<y_vector[2]<<"\n";
		cout<<"z-vector"<<z_vector[0]<<"  "<<z_vector[1]<<"  "<<z_vector[2]<<"\n";

		VariableName="nr of symmetries                ";
	Var1=FindVariable(inf, Section, VariableName);
	nr_symm=Var1.IntData[0];
	cout<<"Num_symm="<<nr_symm<<"\n";

	VariableName="labels                          ";
	Var1=FindVariable(inf, Section, VariableName);
	//labels=new char *[nr_symm];
	labels=Var1.CharData;
	for (int i=0; i<nr_symm; i++)
		cout<<labels[i].c_str()<<"\n";


	VariableName="unrestricted                    ";
	Var1=FindVariable(inf, Section, VariableName);
	Unrestricted=Var1.boolData[0];
	}


};


class Geometry
{
public:
	int nnuc;
	int *qtch;
	double *x;
	double *y;
	double *z;
Geometry ()
{
}
void Geometry1(ifstream inf)
{
	
	std::string Section, VariableName;
	Section="Geometry";
	VariableName="nnuc                            ";
	Variable Var1;
	Var1=FindVariable(inf, Section, VariableName);
	nnuc=Var1.IntData[0];
	
	VariableName="xyznuc                          ";
	Var1=FindVariable(inf, Section, VariableName);
	x=new double [nnuc];
	y=new double [nnuc];
	z=new double [nnuc];
	for (int i=0; i<nnuc; i++)
	{
		x[i]=Var1.doubleData[3*i];
		y[i]=Var1.doubleData[3*i+1];
		z[i]=Var1.doubleData[3*i+2];
	}

	VariableName="qtch                            ";
	Var1=FindVariable(inf, Section, VariableName);
	qtch=new int [nnuc];
	for (i=0; i<nnuc; i++)
		qtch[i]=Var1.doubleData[i];

	
	for (i=0; i<nnuc; i++)
		cout<<qtch[i]<<"  "<<x[i]<<"  "<<y[i]<<"  "<<z[i]<<"\n";
	
	
}
};
class SCF
{

public:
	std::vector <std::string> *MO_numb;
	std::vector <std::string> *MO_numbAR;
	std::vector <std::string> *MO_numbAI;
	std::vector <std::string> *MO_numbBR;
	std::vector <std::string> *MO_numbBI;
	int Relativistic;

	double Emin, Emax;
	std::string lable;
	int *nr_orbitals;
	int nr_points;
	int nSpin;
	double **Occupations;
	double **Eigenvalues;

	/*
	double **MO;

	double **AR;
	double **AI;
	double **BR;
	double **BI;
	*/

	int Spin;
	bool Unrestricted;

	//если без параметра - нерелятивисткий случай
	std::vector<std::string> GenerateVariableName(int RelComp=0)
	{
		std::vector<std::string> VariableName;
		VariableName.resize(0);
		std::string VN;
		
		for (int i=0; i<nr_orbitals[Spin]; i++)
		{
			VN=IntToStr(i+1);
			switch (RelComp)
			{
			case 0: VN+="";
				break;
			case 1: VN+="_alpha_R";
				break;
			case 2: VN+="_beta_R";
				break;
			case 3: VN+="_alpha_I";
				break;
			case 4: VN+="_beta_I";
				break;
			}
		VariableName.push_back(VN);
		}
		return VariableName;
	}


	std::string GenerateSCFSectionName()
	{
	
	std::string SN="SCF_";
	SN+=lable;

	if (Unrestricted)
		{
			if (Spin==0) SN+="_A";
			else SN+="_B";
		}

	cout<<SN.c_str()<<endl;
	return SN;
	};

	std::string GenerateSectionName()
	{
		std::string SN;
		if (Relativistic==0) SN="SCF_";
		else SN="SO_";
		
		SN+=lable;

		if (Unrestricted)
		{
			if (Spin==0) SN+="_A";
			else SN+="_B";
		}
		return SN;

	}

	SCF()
	{
	}
	
	void SCF1(ifstream File, std::string Plable, double PEmin, double PEmax,int RelFlag)
	{
		Relativistic=RelFlag;
		Emin=PEmin;
		Emax=PEmax;
		lable=Plable;
		std::string Section, VariableName;

		Variable Var1;

		
		


//		for (int i=0; i<nSpin; i++)
		
//			Spin=i;
	
			Section=GenerateSCFSectionName();
			

			VariableName="nr of orbitals                  ";
			Var1=FindVariable(File, Section, VariableName);
			nr_orbitals[Spin]=Var1.IntData[0];
			cout<<"nr_orbitals="<<nr_orbitals[Spin]<<"\n";
			VariableName="Occupations                     ";
			Var1=FindVariable(File, Section, VariableName);
			Occupations[Spin]=Var1.doubleData;
			VariableName="Eigenvalues                     ";
			Var1=FindVariable(File, Section, VariableName);
			Eigenvalues[Spin]=Var1.doubleData;
			
			
			if (Relativistic==1)
			{
				MO_numbAR[Spin]=GenerateVariableName(1);
				MO_numbBR[Spin]=GenerateVariableName(2);
				MO_numbAI[Spin]=GenerateVariableName(3);
				MO_numbBI[Spin]=GenerateVariableName(4);

			}
			else
			{
				MO_numb[Spin]=GenerateVariableName();
			}
		
		
	}


/*
	void OpenMO(ifstream File, int CoreNumb)
	{
		std::string Section;
		Section=GenerateSectionName();
		Variable Var1;
		

		if (Relativistic==1)
		{
			Var1=FindVariable(File, Section, MO_numbAR[CoreNumb-1]);
			AR[CoreNumb-1]=Var1.doubleData;
			Var1=FindVariable(File, Section, MO_numbBR[CoreNumb-1]);
			BR[CoreNumb-1]=Var1.doubleData;
			Var1=FindVariable(File, Section, MO_numbAI[CoreNumb-1]);
			AI[CoreNumb-1]=Var1.doubleData;
			Var1=FindVariable(File, Section, MO_numbBI[CoreNumb-1]);
			BI[CoreNumb-1]=Var1.doubleData;

		}
		else
		{
			Var1=FindVariable(File, Section, MO_numb[CoreNumb-1]);
			MO[CoreNumb-1]=Var1.doubleData;
		}
				
		cout<<"MO number "<<CoreNumb<<" was read succesfully"<<endl;

	}

	void OpenMOs(ifstream File)
	{
		std::string Section;
		Section=GenerateSectionName();
		Variable Var1;
		
		for (int i=0; i<nr_orbitals; i++)
		{
			cout<<"Checking MO "<<i+1<<endl;
			cout<<"Energy "<<Eigenvalues[i]<<" Emin="<<Emin<<" Emax="<<Emax<<endl;
			if ((Eigenvalues[i]<Emax)&&(Eigenvalues[i]>Emin))
			{
				
				if (Relativistic==1)
				{
					Var1=FindVariable(File, Section, MO_numbAR[i]);
					AR[i]=Var1.doubleData;
					Var1=FindVariable(File, Section, MO_numbBR[i]);
					BR[i]=Var1.doubleData;
					Var1=FindVariable(File, Section, MO_numbAI[i]);
					AI[i]=Var1.doubleData;
					Var1=FindVariable(File, Section, MO_numbBI[i]);
					BI[i]=Var1.doubleData;
				}
				else
				{

					Var1=FindVariable(File, Section, MO_numb[i]);
					MO[i]=Var1.doubleData;
				}

				nr_points=Var1.size1;
				cout<<"MO number "<<i+1<<" was read succesfully"<<endl;



				//----------------------------------------------------------------






				//----------------------------------------------------------------



			}
		}
	}
*/
};
class Spectrum
{
private: 
	int CoreSymInd;
	int CentralInd;
	int Relativistic;

public:

	double **AR_Core;
	double **BR_Core;
	double **AI_Core;
	double **BI_Core;
	double **MO_Core;
	
	
	
	int AbsEmis;
	char *CoreLable;
	int CoreNumb;
	
	char *FileName;
	
	int AbsAtNumb;
	int AbsIndex;
	double Emin, Emax;
	int PolUnpol;
	double **Polarisation;
	double *CurPolarisation;
	int NumPolar;

	char *OutFileName;

	std::vector <double> Energy;
	std::vector <double> XES;
	std::vector <double *> PolME;
	std::vector <double *> PolXES;
	std::vector <double> BrE;
	std::vector <double> BrXES;

	
	Grid G1;
	Geometry Geom1;
	SCF *S1;
/*
	void SpectrumOld (char *XesInName)
	{
		
		
		ifstream InputFile(XesInName);
		if (!InputFile)
		{
			cout<<"Cannot open file"<<XesInName<<"\n";
			//exit(1);	
		}
		char *FileName;
		char *Comments;
		FileName=new char [82];
		Comments=new char [82];
		//InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>FileName;
		cout<<"FileName="<<FileName<<endl;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>AbsEmis;
		cout<<"AbsEmis="<<AbsEmis<<"\n"<<endl;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		CoreLable=new char [80];
		InputFile>>CoreLable;
		cout<<"CoreLable="<<CoreLable<<endl;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>CoreNumb;
		cout<<"CoreNumb="<<CoreNumb;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>AbsAtNumb;
		cout<<"AbsAtNumb="<<AbsAtNumb<<"\n"<<endl;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>AbsIndex;
		cout<<"AbsIndex="<<AbsIndex<<"\n"<<endl;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>Emin>>Emax;
		cout<<"Emin Emax"<<Emin<<"  "<<Emax<<"\n"<<endl;
		Emin=Emin/27.2;
		Emax=Emax/27.2;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>PolUnpol;
		cout<<"PolUnpol="<<PolUnpol<<endl;
//		Polarisation=new double [3];
		if (PolUnpol==1)
		{
			InputFile.getline(Comments, 80);
			InputFile.getline(Comments, 80);
			InputFile>>NumPolar;
			cout<<"Number of Polarisations"<<NumPolar<<"\n";;
			Polarisation=new double *[NumPolar];
			InputFile.getline(Comments, 80);
			for (int i=0; i<NumPolar; i++)
			{
				Polarisation[i]=new double [3];
				
				InputFile.getline(Comments, 80);
				InputFile>>Polarisation[i][0]>>Polarisation[i][1]>>Polarisation[i][2];
				cout<<"Polarisation ("<<Polarisation[i][0]<<" "<<Polarisation[i][1]<<" "<<Polarisation[i][2]<<")\n"<<endl;
			}

		}
		else 
		{
			NumPolar=3;
			Polarisation=new double *[NumPolar];
			Polarisation[0]=new double [3];
			Polarisation[1]=new double [3];
			Polarisation[2]=new double [3];
			Polarisation[0][0]=1; Polarisation[0][1]=0; Polarisation[0][2]=0;
			Polarisation[1][0]=0; Polarisation[1][1]=1; Polarisation[1][2]=0;
			Polarisation[2][0]=0; Polarisation[2][1]=0; Polarisation[2][2]=1;

		}

		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>Relativistic;


		
		InputFile.close();
		cout<<"End of AdfEmis.inp reading"<<endl;
//----------------------------------------------------------------------------------------------------------
		
		int NumSimb=0;
		while (FileName[NumSimb]!='\0') NumSimb++;
		OutFileName=new char [NumSimb+5];
		OutFileName[0]='X';
		OutFileName[1]='E';
		OutFileName[2]='S';
		OutFileName[3]='-';

		for (int i=0; i<=NumSimb; i++)
			OutFileName[i+4]=FileName[i];

		
		ifstream InpFile(FileName);

		G1.Grid1(InpFile);
		cout<<"End of Grid reading"<<endl;

		FindCoreSymInd();
		S1=new SCF [G1.nr_symm];
		
		for (i=0; i<G1.nr_symm; i++)	
		{
			InpFile.close();
			InpFile.open(FileName);
			
			cout<<"Read orbitals with "<<G1.labels[i].c_str()<<" symmetry "<<endl;


			S1[i].SCF1(InpFile, G1.labels[i],Emin, Emax,Relativistic);
			if (i==CoreSymInd) S1[CoreSymInd].OpenMO(InpFile, CoreNumb);

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			
			S1[i].OpenMOs(InpFile);
		}
			


		Geom1.Geometry1(InpFile);
		cout<<"Geometry was read succesfully"<<endl;
		FindAbsorber();
		cout<<"Absorber="<<CentralInd<<endl;		
		
		InpFile.close();


	}
*/

	Spectrum (char *XesInName)
	{
		
		
		ifstream InputFile(XesInName);
		if (!InputFile)
		{
			cout<<"Cannot open file"<<XesInName<<"\n";
			//exit(1);	
		}
		
		char *Comments;
		FileName=new char [82];
		Comments=new char [82];
		//InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>FileName;
		cout<<"FileName="<<FileName<<endl;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>AbsEmis;
		cout<<"AbsEmis="<<AbsEmis<<"\n"<<endl;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		CoreLable=new char [80];
		InputFile>>CoreLable;
		cout<<"CoreLable="<<CoreLable<<endl;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>CoreNumb;
		cout<<"CoreNumb="<<CoreNumb;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>AbsAtNumb;
		cout<<"AbsAtNumb="<<AbsAtNumb<<"\n"<<endl;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>AbsIndex;
		cout<<"AbsIndex="<<AbsIndex<<"\n"<<endl;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>Emin>>Emax;
		cout<<"Emin Emax"<<Emin<<"  "<<Emax<<"\n"<<endl;
		Emin=Emin/27.2;
		Emax=Emax/27.2;
		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>PolUnpol;
		cout<<"PolUnpol="<<PolUnpol<<endl;
//		Polarisation=new double [3];
		if (PolUnpol==1)
		{
			InputFile.getline(Comments, 80);
			InputFile.getline(Comments, 80);
			InputFile>>NumPolar;
			cout<<"Number of Polarisations"<<NumPolar<<"\n";;
			Polarisation=new double *[NumPolar];
			InputFile.getline(Comments, 80);
			for (int i=0; i<NumPolar; i++)
			{
				Polarisation[i]=new double [3];
				
				InputFile.getline(Comments, 80);
				InputFile>>Polarisation[i][0]>>Polarisation[i][1]>>Polarisation[i][2];
				cout<<"Polarisation ("<<Polarisation[i][0]<<" "<<Polarisation[i][1]<<" "<<Polarisation[i][2]<<")\n"<<endl;
			}

		}
		else 
		{
			NumPolar=3;
			Polarisation=new double *[NumPolar];
			Polarisation[0]=new double [3];
			Polarisation[1]=new double [3];
			Polarisation[2]=new double [3];
			Polarisation[0][0]=1; Polarisation[0][1]=0; Polarisation[0][2]=0;
			Polarisation[1][0]=0; Polarisation[1][1]=1; Polarisation[1][2]=0;
			Polarisation[2][0]=0; Polarisation[2][1]=0; Polarisation[2][2]=1;

		}

		InputFile.getline(Comments, 80);
		InputFile.getline(Comments, 80);
		InputFile>>Relativistic;


		
		InputFile.close();
		cout<<"End of AdfEmis.inp reading"<<endl;
//----------------------------------------------------------------------------------------------------------
		
		int NumSimb=0;
		while (FileName[NumSimb]!='\0') NumSimb++;
		OutFileName=new char [NumSimb+5];
		OutFileName[0]='X';
		OutFileName[1]='E';
		OutFileName[2]='S';
		OutFileName[3]='-';

		for (int i=0; i<=NumSimb; i++)
			OutFileName[i+4]=FileName[i];

		
		ifstream InpFile(FileName);

		G1.Grid1(InpFile);
		cout<<"End of Grid reading"<<endl;


		FindCoreSymInd();
		S1=new SCF [G1.nr_symm];
		
		
		//!!!!! Непонятно зачем файл для каждой симметрии читать сначала
		//Наверное с начала нужно начинать после чтения остовной функции в релятивистском случае.


		for (i=0; i<G1.nr_symm; i++)	
		{
			InpFile.close();
			InpFile.open(FileName);
			
			cout<<"Read orbitals with "<<G1.labels[i].c_str()<<" symmetry "<<endl;


			S1[i].Unrestricted=G1.Unrestricted;
			
			
			if ((G1.Unrestricted)&&(!Relativistic)) S1[i].nSpin=2;
			else S1[i].nSpin=1;
			int nS=S1[i].nSpin;

			
			
			
			S1[i].nr_orbitals=new int[nS];
			S1[i].Occupations=new double *[nS];
			S1[i].Eigenvalues=new double *[nS];

			S1[i].MO_numbAR=new std::vector <std::string> [nS];
			S1[i].MO_numbBR=new std::vector <std::string> [nS];
			S1[i].MO_numbAI=new std::vector <std::string> [nS];
			S1[i].MO_numbBI=new std::vector <std::string> [nS];
			S1[i].MO_numb=new std::vector <std::string> [nS];
	
			
			
			if (i==CoreSymInd) 
				{
					AR_Core=new double *[S1[i].nSpin];
					BR_Core=new double *[S1[i].nSpin];
					AI_Core=new double *[S1[i].nSpin];
					BI_Core=new double *[S1[i].nSpin];
					MO_Core=new double *[S1[i].nSpin];
				}
			
			for (int ispin=0; ispin<S1[i].nSpin; ispin++)
			{
				S1[i].Spin=ispin;
				S1[i].SCF1(InpFile, G1.labels[i],Emin, Emax,Relativistic);
				
				
				if (i==CoreSymInd) 
				{
					ReadCore(InpFile);
				}

			}
		}
			


		Geom1.Geometry1(InpFile);
		cout<<"Geometry was read succesfully"<<endl;
		FindAbsorber();
		cout<<"Absorber="<<CentralInd<<endl;		
		
		InpFile.close();


	}





	void FindAbsorber()
	{
		int Numb=0;
		for (int i=0; i<Geom1.nnuc; i++)
		{
			if (Geom1.qtch[i]==AbsAtNumb)
			{
				Numb=Numb+1;
				if (Numb==AbsIndex) CentralInd=i;
			}
		}
	}
	void FindCoreSymInd()
	{
		int NumSimbC=0;
		while (CoreLable[NumSimbC]!='\0') NumSimbC++;

		for (int i=0; i<G1.nr_symm; i++)
		{
			int flag=1;
			int NumSimb=0;
			while (G1.labels[i][NumSimb]!='\0') NumSimb++;
			if (NumSimbC==NumSimb)
			{
				for (int j=0; j<NumSimb; j++)
					if (G1.labels[i][j]!=CoreLable[j]) flag=0;
			}
			else flag=0;
			if (flag==1) CoreSymInd=i;
		}
		cout<<"CoreSymInd="<<CoreSymInd<<"\n";
	}


	void ReadCore(ifstream File)
	{

		

		int GridSize=G1.nr_points_x*G1.nr_points_y*G1.nr_points_z;

		int Spin=S1[CoreSymInd].Spin;
		if (Relativistic==1)
		{
			
			AR_Core[Spin]=new double[GridSize];
			BR_Core[Spin]=new double[GridSize];
			AI_Core[Spin]=new double[GridSize];
			BI_Core[Spin]=new double[GridSize];

		}
		else
			{
			
			MO_Core[Spin]=new double[GridSize];
			}


				std::string Section;
				Section=S1[CoreSymInd].GenerateSectionName();
		
				if (Relativistic==1)

				{
					FindVariable(File, Section, S1[CoreSymInd].MO_numbAR[Spin][CoreNumb-1],AR_Core[Spin]);
					FindVariable(File, Section, S1[CoreSymInd].MO_numbBR[Spin][CoreNumb-1],BR_Core[Spin]);
					FindVariable(File, Section, S1[CoreSymInd].MO_numbAI[Spin][CoreNumb-1],AI_Core[Spin]);
					FindVariable(File, Section, S1[CoreSymInd].MO_numbBI[Spin][CoreNumb-1],BI_Core[Spin]);
				}
				else
				{
					FindVariable(File, Section, S1[CoreSymInd].MO_numb[Spin][CoreNumb-1],MO_Core[Spin]);
				}

	}
	
	void ReadAndCalc()
	{

		ifstream File(FileName);


		
		
		std::string Section;
		double *AR_Current;
		double *BR_Current;
		double *AI_Current;
		double *BI_Current;
		double *MO_Current;

		int GridSize=G1.nr_points_x*G1.nr_points_y*G1.nr_points_z;

		if (Relativistic==1)
		{
			AR_Current=new double[GridSize];
			BR_Current=new double[GridSize];
			AI_Current=new double[GridSize];
			BI_Current=new double[GridSize];

		}
		else
			MO_Current=new double[GridSize];
		
		
		double deltaV;
		deltaV=G1.x_vector[0]*(G1.y_vector[1]*G1.z_vector[2]-G1.y_vector[2]*G1.z_vector[1])-G1.x_vector[1]*(G1.y_vector[0]*G1.z_vector[2]-G1.y_vector[2]*G1.z_vector[0])+G1.x_vector[2]*(G1.y_vector[0]*G1.z_vector[1]-G1.y_vector[1]*G1.z_vector[0]);
		
				
			
		for (int sy=0; sy<G1.nr_symm; sy++)
		{
			
			for (int ispin=0; (ispin<(S1[sy].nSpin)); ispin++)
			{
				cout<<"SPIN="<<ispin<<endl;
				S1[sy].Spin=ispin;

			
			
				Section=S1[sy].GenerateSectionName();
				
				for (int ei=0; ei<(S1[sy].nr_orbitals[ispin]); ei++)
				{
					cout<<"MO "<<ei+1<<"  ";
				//	cout<<"Energy "<<S1[sy].Eigenvalues[ispin][ei]<<" Emin="<<Emin<<" Emax="<<Emax<<endl;
					if ((S1[sy].Eigenvalues[ispin][ei]<Emax)&&(S1[sy].Eigenvalues[ispin][ei]>Emin))
					{
						
						if (Relativistic==1)
						{
							FindVariable(File, Section, S1[sy].MO_numbAR[ispin][ei],AR_Current);
							FindVariable(File, Section, S1[sy].MO_numbBR[ispin][ei],BR_Current);
							FindVariable(File, Section, S1[sy].MO_numbAI[ispin][ei],AI_Current);
							FindVariable(File, Section, S1[sy].MO_numbBI[ispin][ei],BI_Current);
						}
						else
						{
							FindVariable(File, Section, S1[sy].MO_numb[ispin][ei],MO_Current);
						}

						S1[sy].nr_points=GridSize;
						cout<<"MO number "<<ei+1<<" was read succesfully"<<endl;


					//---------------------------------------------------------------------------
						double x,y,z,r_x, r_y, r_z, er,x_abs, y_abs, z_abs;
						x_abs=Geom1.x[CentralInd];
						y_abs=Geom1.y[CentralInd];
						z_abs=Geom1.z[CentralInd];
				
						
						
						double *SP;
						SP=G1.Start_point;

						double *CurrentXES;
						CurrentXES=new double [NumPolar];
						double *CurrentPolME;
						CurrentPolME=new double [NumPolar];

						
						for (int polk=0; polk<NumPolar; polk++)
						{
							
							int PN=0;
							double S=0;
							double SReal=0;
							double SImag=0;
							
							CurPolarisation=Polarisation[polk];
							//Grid cycle
							for (int i=0; i<G1.nr_points_z; i++)
							{
								for (int j=0; j<G1.nr_points_y; j++)
								{
									for (int k=0; k<G1.nr_points_x; k++)
									{
									
										

										x=G1.Start_point[0]+G1.x_vector[0]*k+G1.y_vector[0]*j+G1.z_vector[0]*i;
										y=G1.Start_point[1]+G1.x_vector[1]*k+G1.y_vector[1]*j+G1.z_vector[1]*i;
										z=G1.Start_point[2]+G1.x_vector[2]*k+G1.y_vector[2]*j+G1.z_vector[2]*i;
										r_x=x-x_abs;
										r_y=y-y_abs;
										r_z=z-z_abs;
										er=r_x*CurPolarisation[0]+r_y*CurPolarisation[1]+r_z*CurPolarisation[2];
										double a,aReal, aImag;
										
										
										//SCF SCF1;
										//SCF1=S1[CoreSymInd];
										
									//	float qwewqwe=MO_Core[0][0];
									//	float qwewqwe2=MO_Current[0];


										if (Relativistic==1)
										{
											
											
											aReal=AR_Current[PN]*er*AR_Core[ispin][PN]+
												  AI_Current[PN]*er*AI_Core[ispin][PN]+
												  BR_Current[PN]*er*BR_Core[ispin][PN]+
												  BI_Current[PN]*er*BI_Core[ispin][PN];

											aImag=AR_Current[PN]*er*AI_Core[ispin][PN]-
												  AI_Current[PN]*er*AR_Core[ispin][PN]+
												  BR_Current[PN]*er*BI_Core[ispin][PN]-
												  BI_Current[PN]*er*BR_Core[ispin][PN];


											SReal=SReal+aReal;
											SImag=SImag+aImag;


										}
										else
										{
										
											a=MO_Core[ispin][PN]*er*MO_Current[PN];
											S=S+a;
										}
										
										PN=PN+1;
										
									}
								}
							}

							//End of Grid Cycle
						
							if (Relativistic==1)
								S=sqrt(SReal*SReal+SImag*SImag)*deltaV;
							else S=S*deltaV;
						
							
							if (AbsEmis==1) CurrentXES[polk]=S1[sy].Occupations[ispin][ei]*S*S;
							else	if ((Relativistic==1)||(G1.Unrestricted)) CurrentXES[polk]=(1-S1[sy].Occupations[ispin][ei])*S*S;
									else CurrentXES[polk]=(2-S1[sy].Occupations[ispin][ei])*S*S;
							CurrentPolME[polk]=S;
						
						
												
						} //End of polarization cycle
						Energy.push_back(S1[sy].Eigenvalues[ispin][ei]);
						PolXES.push_back(CurrentXES);
						PolME.push_back(CurrentPolME);

					
					}
				}
			}

		}
		
	File.close();
	}
	
	//В релятевистком случае расчитывает не матричный элемент а его модуль 	
/*
	double CalcME(int SymInd, int OrbInd)
	{
		double x,y,z,r_x, r_y, r_z, er,x_abs, y_abs, z_abs;
		x_abs=Geom1.x[CentralInd];
		y_abs=Geom1.y[CentralInd];
		z_abs=Geom1.z[CentralInd];
		
		int PN=0;
		double S=0;
		double SReal=0;
		double SImag=0;
		for (int i=0; i<G1.nr_points_z; i++)
		{
			for (int j=0; j<G1.nr_points_y; j++)
			{
				for (int k=0; k<G1.nr_points_x; k++)
				{
					double *SP;
					SP=G1.Start_point;
					
					//cout<<"SP="<<SP[0]<<"\n";
					//char qwe;
					//cin>>qwe;

					x=G1.Start_point[0]+G1.x_vector[0]*k+G1.y_vector[0]*j+G1.z_vector[0]*i;
					y=G1.Start_point[1]+G1.x_vector[1]*k+G1.y_vector[1]*j+G1.z_vector[1]*i;
					z=G1.Start_point[2]+G1.x_vector[2]*k+G1.y_vector[2]*j+G1.z_vector[2]*i;
					
					r_x=x-x_abs;
					r_y=y-y_abs;
					r_z=z-z_abs;
					er=r_x*CurPolarisation[0]+r_y*CurPolarisation[1]+r_z*CurPolarisation[2];
					double a,aReal, aImag;
					//double a1, a2;
					SCF SCF1;
					SCF1=S1[CoreSymInd];
					
					if (Relativistic==1)
					{
						
						
						aReal=S1[SymInd].AR[OrbInd][PN]*er*S1[CoreSymInd].AR[CoreNumb-1][PN]+
							  S1[SymInd].AI[OrbInd][PN]*er*S1[CoreSymInd].AI[CoreNumb-1][PN]+
							  S1[SymInd].BR[OrbInd][PN]*er*S1[CoreSymInd].BR[CoreNumb-1][PN]+
							  S1[SymInd].BI[OrbInd][PN]*er*S1[CoreSymInd].BI[CoreNumb-1][PN];

						aImag=S1[SymInd].AR[OrbInd][PN]*er*S1[CoreSymInd].AI[CoreNumb-1][PN]-
							  S1[SymInd].AI[OrbInd][PN]*er*S1[CoreSymInd].AR[CoreNumb-1][PN]+
							  S1[SymInd].BR[OrbInd][PN]*er*S1[CoreSymInd].BI[CoreNumb-1][PN]-
							  S1[SymInd].BI[OrbInd][PN]*er*S1[CoreSymInd].BR[CoreNumb-1][PN];


						SReal=SReal+aReal;
						SImag=SImag+aImag;


					}
					else
					{
					
					//	a1=SCF1.MO[CoreNumb-1][PN];
					//	a2=S1[SymInd].MO[OrbInd][PN];
						a=S1[CoreSymInd].MO[CoreNumb-1][PN]*er*S1[SymInd].MO[OrbInd][PN];
						S=S+a;
					}
					
					PN=PN+1;
					
				}
			}
		}
		
		double deltaV;
		deltaV=G1.x_vector[0]*(G1.y_vector[1]*G1.z_vector[2]-G1.y_vector[2]*G1.z_vector[1])-G1.x_vector[1]*(G1.y_vector[0]*G1.z_vector[2]-G1.y_vector[2]*G1.z_vector[0])+G1.x_vector[2]*(G1.y_vector[0]*G1.z_vector[1]-G1.y_vector[1]*G1.z_vector[0]);
		//S=0;
		if (Relativistic==1)
			S=sqrt(SReal*SReal+SImag*SImag)*deltaV;
			else S=S*deltaV;
		return S;
	}
*/
	
	/*	
	void CalcPolarXES()
	{
		for (int i=0; i<G1.nr_symm; i++)
			for (int j=0; j<S1[i].nr_orbitals; j++)
				if ((S1[i].Eigenvalues[j]<=Emax)&&(S1[i].Eigenvalues[j]>=Emin))
				{
					double *CurrentXES;
					CurrentXES=new double [NumPolar];
					double *CurrentPolME;
					CurrentPolME=new double [NumPolar];
					double Me;
					
					
					for (int k=0; k<NumPolar; k++)
					{
						cout<<"ijk"<<i<<"  "<<j<<"  "<<k<<endl;
						CurPolarisation=Polarisation[k];
						Me=CalcME(i,j);
						
						if (AbsEmis==1) CurrentXES[k]=S1[i].Occupations[j]*Me*Me;
						else	if (Relativistic==1)CurrentXES[k]=(1-S1[i].Occupations[j])*Me*Me;
								else CurrentXES[k]=(2-S1[i].Occupations[j])*Me*Me;
						
						CurrentPolME[k]=Me;
					}
					Energy.push_back(S1[i].Eigenvalues[j]);
					PolXES.push_back(CurrentXES);
					PolME.push_back(CurrentPolME);
				}
	}
*/	
	void CalcUnpolarXES()
	{
		int N=PolXES.size();
		XES.resize(N);
		for (int i=0; i<N; i++)
			XES[i]=(PolXES[i][0]+PolXES[i][1]+PolXES[i][2])/3;	
	}
	void SaveUnpolarXES()
	{
	//	double CoreE=S1[CoreSymInd].Eigenvalues[CoreNumb-1];
		ofstream OutFile(OutFileName);
		ofstream OutFileME("me.txt");
		OutFile<<NumPolar+2<<"  "<<Energy.size()<<"\n";
		OutFileME<<NumPolar+1<<"  "<<Energy.size()<<"\n";
		for (int i=0; i<Energy.size(); i++)
		{
			double e=(Energy[i])*27.2;
			OutFile<<e;
			OutFileME<<e;
			for (int k=0; k<NumPolar; k++)
			{
				OutFile<<"  "<<PolXES[i][k];
				OutFileME<<"  "<<PolME[i][k];
			}
			OutFile<<"  "<<XES[i];
			OutFile<<"\n";
			OutFileME<<"\n";
		}
		OutFile.close();
		OutFileME.close();
	}

	void SaveXES()
	{
	//	double CoreE=S1[CoreSymInd].Eigenvalues[CoreNumb-1];
		ofstream OutFile(OutFileName);
		ofstream OutFileME("me.txt");
		OutFile<<NumPolar+1<<"  "<<Energy.size()<<"\n";
		OutFileME<<NumPolar+1<<"  "<<Energy.size()<<"\n";
		for (int i=0; i<Energy.size(); i++)
		{
			double e=(Energy[i])*27.2;
			OutFile<<e;
			OutFileME<<e;
			for (int k=0; k<NumPolar; k++)
			{
				OutFile<<"  "<<PolXES[i][k];
				OutFileME<<"  "<<PolME[i][k];
			}
			OutFile<<"\n";
			OutFileME<<"\n";
		}
		OutFile.close();	
		OutFileME.close();	
	}

/*	void SaveXES()
	
	{
		ofstream OutFile(OutFileName);
		for (int i=0; i<Energy.size(); i++)
			OutFile<<Energy[i]<<" "<<XES[i]<<"\n";
		OutFile.close();

		ofstream OutFile1("brXES.dat");
		for ( i=0; i<BrE.size(); i++)
			OutFile1<<BrE[i]<<" "<<BrXES[i]<<"\n";
		OutFile1.close();
	}
	*/
/*	void Broad(double w)
	{
		int n=400;
		for (int i=0; i<n; i++)
		{
			double E,h;
			h=(Emax-Emin)/n;
			E=Emin+h*i;
			double S=0;
			int m=Energy.size();
			for (int j=0; j<m; j++)
			{
				S=S+XES[j]*Lorentz(E,Energy[j],w);
			}
			BrE.push_back(E);
			BrXES.push_back(S);
		}
	}
*/
//	double Lorentz(double x,double x0,double w)
//	{
//		if (w==0) throw "Lorentz width=0";
//		double l;
//		double Pi=3.1415926;
//	//	l=2/**w*//(Pi*(4*(x-x0)*(x-x0)+w*w));
//		return l;
//	}

};
/*void mainOLD()
{
	cout<<"Start Reading"<<endl;
	char *EmisAdfInp="EmisAdf.inp";
	Spectrum Spec1(EmisAdfInp);
	cout<<"Start Calculations"<<endl;
	Spec1.CalcPolarXES();
	cout<<"Start Saving"<<endl;
	if (Spec1.PolUnpol==1) Spec1.SaveXES();
	else
	{
		Spec1.CalcUnpolarXES();
		Spec1.SaveUnpolarXES();
	}
	
		
//char *T41name="dmpkfout.txt";
//		ifstream T41File(T41name);


//Grid G1;
//G1.Grid1(T41File);
//Geometry G2;
//G2.Geometry1(T41File);


}

*/
void main()
{
	cout<<"Start Reading symmetry, energies, coordinates"<<endl;
	char *EmisAdfInp="EmisAdf.inp";
	Spectrum Spec1(EmisAdfInp);

	cout<<"Start read MO and calculate"<<endl;

	Spec1.ReadAndCalc();
	cout<<"Start Saving"<<endl;
	if (Spec1.PolUnpol==1) Spec1.SaveXES();
	else
	{
		Spec1.CalcUnpolarXES();
		Spec1.SaveUnpolarXES();
	}
	
	

}

