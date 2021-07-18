
#include <random>
#include <octetos/core/Error.hh>
#include <iostream>
#include <algorithm>
#include <ctime>
//#include <libtar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <octetos/coreutils/shell.hh>
#include <fstream>

#include "core.hh"


namespace ec
{


bool cmpStrength(const Single* f,const Single* s)
{
	return f->getFitness() > s->getFitness();
}
bool cmpStrength1(const Single* f,const Single* s)
{
	return std::abs(f->getFitness() - 1.0) > std::abs(s->getFitness() - 1.0);
}
double randNumber()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> distr(0.0, 1.0);
	
	return distr(gen);
}
double randNumber(double max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> distr(0.0, max);
	
	return distr(gen);
}
double randNumber(double min,double max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> distr(min, max);
	
	return distr(gen);
}
struct caret4B_half
{
	short a:16,b:16;
};
struct caret2B_half
{
	unsigned short a:8,b:8,c:4;
};

struct caret2B_half_Digits1
{
	unsigned short a:4,e:12;
};
struct caret2B_half_Digits2
{
	unsigned short a:2,b:2,e:12;
};
struct caret2B_half_Digits3
{
	unsigned short a:1,b:1,c:1,d:1,e:12;
};







	
Chromosome::Chromosome(const std::string n) : name(n)
{
}
Chromosome::~Chromosome()
{

}
const Chromosome& Chromosome::operator = (const Chromosome& obj)
{
	name = obj.name;

	return *this;
}














Junction::Junction(): Chromosome("Junction")
{
	number = randChild();
	algorit = randAlgt();
}
Junction::Junction(geneUS n,geneUS a): Chromosome("Junction")
{
	number = n;
	algorit = a;
}
Junction::~Junction()
{

}
geneUS Junction::get_number()const
{
	return number;
}
geneUS Junction::get_algorit()const
{
	return algorit;
}

void Junction::combine(const Chromosome& P1,const Chromosome& P2)
{
	copy(P1);
}

void Junction::copy(const Chromosome& P)
{
	number = ((Junction&)P).number;
	algorit = ((Junction&)P).algorit;
}
void Junction::mutate(float p)
{
	double numrd1 = randNumber(0.0,1.0);
	if(numrd1 <= p) number = randNumber(1.0,10.0);
	numrd1 = randNumber(0.0,1.0);
	if(numrd1 <= p) algorit = randAlgt();
}
geneUS Junction::randAlgt()
{
	double randNum = randNumber();
	if(randNum < 0.01) return COPY;

	return COMBINE;
}
geneUS Junction::randChild()
{
	return randNumber(1.0, 5.0);
}
void Junction::randFill(bool favor)
{
	number = randChild();
	algorit = randAlgt();
}














Single::Single(const Single&)
{
	throw octetos::core::Exception("Operacion invalida.",__FILE__,__LINE__);
}
Single::Single(ec::ID id,Enviroment& e) : env(&e)
{
	this->id = id;
	init();
}
Single::Single(ec::ID id,Enviroment& e,const Junction& j) : junction(j),env(&e)
{
	this->id = id;
	init();
}
Single::~Single()
{

}


ID Single::getID()const
{
	return id;
}
unsigned short Single::getAge() const
{
	return age;
}
double Single::getFitness() const
{
	return fitness;
}
const Junction& Single::getJunction()const
{
	return junction;
}
Enviroment& Single::getEnviroment()const
{
	return *env;
}

bool Single::mudable()const
{
	double numrand = randNumber(0.0,1.0);
	if(numrand <= env->getProbabilityMutationEvent()) return true;
	else return false;	
}
void Single::deltaAge()
{
	age++;
}
void Single::init()
{
	age = 0;
	fitness = 0;
}








void Enviroment::init()
{
	idCount = 1;
	actualIteration = 0;
	maxIteration = 0;
	echolevel = 0;
	logFile = false;
	//sigmaReduction = 1.0;
	minSolutions = 0;
	pMutationEvent = 0.02;
	pMutableGene = 0.4;
	fout = NULL;
	stopMaxIterations=enableMinSolutions=false;
	percen_at_iteration = 0.4;//%
	comparer = NULL;
}
Enviroment::Enviroment()
{
	init();	
}
Enviroment::~Enviroment()
{

}

Enviroment::Enviroment(Iteration m) : maxIteration(m)
{
	init();	
}
Enviroment::Enviroment(const std::string& log,Iteration m) : maxIteration(m), logDirectory(log)
{
	init();		
}

Population Enviroment::getMaxPopulation()const
{
	return maxPopulation;
}
Population Enviroment::getInitPopulation()const
{
	return initPopulation;
}
Population Enviroment::getMaxProgenitor()const
{
	return maxProgenitor;
}
double Enviroment::getSigma() const
{
	return sigma;
}
double Enviroment::Enviroment::getMedia() const
{
	return media;
}
double Enviroment::getEpsilon() const
{
	return epsilon;
}
double Enviroment::getProbabilityMutableGene()const
{
	return pMutableGene;
}
double Enviroment::getProbabilityMutationEvent()const
{
	return pMutationEvent;
}
unsigned long Enviroment::getSession()const
{
	std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);

	unsigned long v = (now->tm_year + 1900) * 10000000000;
	v += (now->tm_mon + 1 ) * 100000000;
	v += now->tm_mday * 1000000;
	v += now->tm_hour * 10000;
	v += now->tm_min * 100;
	v += now->tm_sec;
	
    return v;
}
unsigned long Enviroment::getDayID()
{
	std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);

	unsigned long v = (now->tm_year + 1900) * 10000;
	v += (now->tm_mon + 1 ) * 100;
	v += now->tm_mday;
	
    return v;
}
unsigned long Enviroment::getTimeID()
{
	std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);

	unsigned long v = now->tm_hour * 10000;
	v += now->tm_min * 100;
	v += now->tm_sec;
	
    return v;
}
ID Enviroment::getCountID()
{
	return idCount;
}
std::ostream* Enviroment::getFout()
{
	return fout;
}
unsigned short Enviroment::getEchoLevel()const
{
	return echolevel;
}
const std::string Enviroment::getLogSubDirectory()const
{
	return logSubDirectory;
}


ID Enviroment::nextID()
{
	return ++idCount;
}
void Enviroment::enableEcho(std::ostream* f, unsigned short level)
{
	fout = f;
	echolevel = level;
}

void Enviroment::enableLogFile(bool log)
{
	logFile = log;
}
bool Enviroment::run()
{	
	//std::cout << "\tStep 0\n";
	
	if(maxProgenitor < minSolutions) throw octetos::core::Exception("La cantidad de progenoore deveria ser major que la cantidad de soluciones buscadas",__FILE__,__LINE__);
	actualIteration = 1;
	
	//std::cout << "\tStep 1\n";
	initial();
	//std::cout << "\tStep 2\n";
	unsigned short counUndelete = 0;
	std::ofstream history;
	//std::cout << "\tStep 3\n";
	logFile = not logDirectory.empty();
	if(logFile)
	{
		session = getSession();
		logSubDirectory = logDirectory +"/" + std::to_string(getTimeID());
		std::string strhistory = logSubDirectory + "/historial.csv";
		coreutils::Shell shell;
		shell.mkdir(logSubDirectory);
		history.open(strhistory);
	}
	//std::cout << "\tStep 4\n";

	ID oldleaderID = 0;
	double oldLeaderFitness = 0.0;
	Iteration countOldLeader = 0;
	Iteration countOldLeaderFitness = 0;
	//std::cout << "\tStep 5\n";
	while(true)
	{
		//std::cout << "\tStep C1\n";
		if(stopMaxIterations) 
		{
			if(actualIteration > maxIteration) 
			{
				history.close();
				return false;
			}
		}
		if(echolevel > 0 and fout != NULL) 
		{
			if(maxIteration > 0) (*fout) << ">>> Iteracion : " << actualIteration << "/" << maxIteration << "\n";
			else (*fout) << ">>> Iteracion : " << actualIteration << "\n";
		}
		//std::cout << "\tStep C2\n";
		
		media = 0.0;
		sigma = 0.0;
		
		//std::cout << "\tStep C3\n";
		eval();
		//std::cout << "\tStep C4\n";
		sort(comparer);
		//std::cout << "\tStep C5\n";
		
		ec::ID countBefore = size();
		selection();
		//std::cout << "\tStep C6\n";
		if(logFile)
		{
			std::string strSelection = logSubDirectory +  "/selection-" + std::to_string(actualIteration) + ".csv";
			std::ofstream fnSelection(strSelection);
			if(not fnSelection.is_open()) throw octetos::core::Exception("No se logro abrir el archivo",__FILE__,__LINE__);
			for(ec::Single* s : *this)
			{
				s->save(fnSelection);
			}
			fnSelection.flush();
			fnSelection.close();
		}
		//std::cout << "\tStep C7\n";
		unsigned short removes = countBefore - size();
		//deletes == 0 ? counUndelete++ : counUndelete = 0;
		if(echolevel > 1 and fout != NULL) 
		{
			(*fout) << "\tTamano de la poblacion : " << size() << "\n";
			(*fout) << "\tProgenitores selecionados, total : " << size() << "\n";
			(*fout) << "\tEliminados : " << removes << "\n";	
		}
		//std::cout << "\tStep C8\n";
		
		for(ec::Single* s : *this)
		{
			//std::cout << "\t" << s->getID() << " Fortaleza : " << s->getStrength() << "\n";
			media += s->getFitness();
			s->deltaAge();			
		}
		media /= size();
		for(ec::Single* s : *this)
		{
			//std::cout << "\t" << s->getID() << " Fortaleza : " << s->getStrength() << "\n";
			sigma += pow(s->getFitness() - media,2);
		}
		sigma /= size();

		if(logFile)
		{
			std::string strfn = logSubDirectory +  "/iteracion-" + std::to_string(actualIteration) + ".csv";
			std::ofstream fn(strfn);
			if(not fn.is_open()) throw octetos::core::Exception("No se logro abrir el archivo",__FILE__,__LINE__);
			for(ec::Single* s : *this)
			{
				s->save(fn);
			}
			fn.flush();
			fn.close();
		}
		ec::Single* leader = *begin();
		if(echolevel > 1 and fout != NULL) 
		{			
			std::cout << "\tLider : " << leader->getFitness() << "\n";
			(*fout) << "\tmedia : " << media << "\n";
			(*fout) << "\tDesviacion estandar : " << sigma << "\n";
			//(*fout) << "\tVariables faltantes : " << getFaltantes() << "\n";
		}
		
		if(stopNotDiference and actualIteration > 1)
		{
			if(sigma < notDiferenceCota)
			{
				std::cout << "\tFinalizando por no diferencia\n";
				history.close();
				return false;
			}
		}
		if(enableMinSolutions)
		{
			Population countSols = 0;
			Iteration counItr = 0;
			ec::Single* s;
			for(iterator it = begin(); it != end() and counItr <= minSolutions; it++,counItr++)
			{
				s = *it;
				if(1.0 - s->getFitness () < Enviroment::epsilon)
				{
					countSols++;
					if(countSols >= minSolutions)
					{
						if(echolevel > 0 and fout != NULL) (*fout) << "\n\tSe completo el conjunto de solucion minimo\n";
						//s->print((*fout));
						if(logFile) save();
						//compress(logDir,logDir+".tar");
						std::cout << "\tFinalizado devido a solucion minima encontrada\n";
						history.close();
						return true;
					}
				}
			}
		}
		//std::cout <<  "Step 5\n";

		if(logFile)
		{
			if(history.is_open()) 
			{
				history  << actualIteration;
				history  << ",";
				history  << size();
				history  << ",";
				history  << media;
				history  << ",";
				history  << sigma;	
				history  << ",";
				history  << pMutationEvent;
				history  << ",";
				history  << pMutableGene;
				//history  << ",";
				//history  << getFaltantes();
				history  << "\n";
				history .flush();
			}
		}
		//std::cout <<  "Step 6\n";
				
		juncting();			
		for(ec::Single* s : newschils)//agregar los nuevos hijos a la poblacion
		{
			push_front(s);		
		}
		if(logFile)
		{
			std::string strChilds = logSubDirectory + "/hijos-" + std::to_string(actualIteration) + ".csv";
			std::ofstream fnChilds(strChilds);
			if(not fnChilds.is_open()) throw octetos::core::Exception("No se logro abrir el archivo",__FILE__,__LINE__);
			for(ec::Single* s : newschils)//agregar los nuevos hijos a la poblacion
			{
				s->save(fnChilds);			
			}
			fnChilds.flush();
			fnChilds.close();
		}
		if(echolevel > 1 and fout != NULL) 
		{
			(*fout) << "\tNuevos Hijos : " << newschils.size() << "\n";
		}
		newschils.clear();		
		
		actualIteration++;
	}

	return false;
}

void Enviroment::stopperMaxIterations(Iteration max)
{
	maxIteration = max;
	stopMaxIterations = true;
}
void Enviroment::stopperNotDiference(double cota)
{
	notDiferenceCota = cota;
	stopNotDiference = true;
}



ec::Single* Enviroment::getProxSolution()
{
	ec::Single* s;
	for(iterator it = begin(); it != end(); it++)
	{
		s = *it;
		if(1.0 - s->getFitness() > epsilon) return s;
	}
	
	return *begin();
}

Single* Enviroment::getRandomSingleTop() const
{
	float maxp = std::distance(begin(),end());
	const_iterator it = begin();
	
	double rndnum = randNumber(0.0,1.0);
	if(rndnum < 0.30)
	{
		return *begin();
	}
	else if(rndnum < 0.60)
	{
		return *begin()++;
	}
	
	return NULL;
}
Single* Enviroment::getRandomSingle() const
{
	float maxp = std::distance(begin(),end());
	const_iterator it = begin();
	
	double rndnum = randNumber(0.0,maxp);
	std::advance(it,rndnum);
	if(it != end()) return *it;
	
	return NULL;
}
void Enviroment::juncting()
{
	//std::cout << "Enviroment::juncting Step 1\n";
	Single *single1,*single2;
	//std::cout << "Enviroment::juncting Step 2\n";
	do
	{
		//std::cout << "Enviroment::juncting Step 2.1\n";		
		ec::Single* single1 = getRandomSingle();
		if(single1 == NULL) continue;
		ec::Single* single2 = getRandomSingle();
		if(single2 == NULL) continue;
		if(single1 == single2) continue;
		//std::cout << "Enviroment::juncting Step 2.2\n";
		single1->juncting(newschils,single2,echolevel,NULL);
		//std::cout << "Enviroment::juncting Step 2.3\n";
		if(echolevel > 2 and fout != NULL) (*fout) << "\tSe ha unido " << single1->getID() << " con " << single2->getID() << "\n";
	}
	while(newschils.size() + size() <= maxPopulation);
	//std::cout << "Enviroment::juncting Step 3\n";
}
void Enviroment::eval()
{
	Single* single;
	for(ec::Single* s : *this)
	{
		single = (Single*) s;
		single->eval();
	}
}
}