
#include <cfloat>
#include <random>
#include <iostream>
#include <algorithm>
#include <ctime>
//#include <libtar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <iomanip>
//#include <ctime>
#include <fstream>
#include <algorithm>
#if defined(__linux__)
    #include <octetos/math/statics.hh>
#elif defined(_WIN32) || defined(_WIN64)
    #include <math/src/statics.hh>
#else
    #error "Pltaforma desconocida"
#endif

#include "ec.hh"


namespace oct::ec
{


bool cmpStrength(const Single* f,const Single* s)
{
	return f->getFitness() > s->getFitness();
}
bool cmpStrength1(const Single* f,const Single* s)
{
	return std::abs(f->getFitness() - 1.0) > std::abs(s->getFitness() - 1.0);
}
/*
double randNumber()
{
	//std::random_device rd;
	//std::mt19937 gen(rd());
	std::uniform_real_distribution<double> distr(0.0, 1.0);

	return distr(gen);
}
double randNumber(double max)
{
	//std::random_device rd;
	//std::mt19937 gen(rd());
	std::uniform_real_distribution<double> distr(0.0, max);

	return distr(gen);
}
double randNumber(double min,double max)
{
	//std::random_device rd;
	//std::mt19937 gen(rd());
	std::uniform_real_distribution<double> distr(min, max);

	return distr(gen);
}
*/


Exception::Exception() : code(UNKNOW)
{
}
Exception::Exception(Code c,const char* fn, unsigned int l) : code(c),filename(fn),line(l)
{
}
const char* Exception::what() const throw()
{
	switch(code)
	{
		case UNKNOW:
			return "Error desconocido";
		case BAD_VALUE_maxProgenitor:
			return "Valor incorrecto de maxProgenitor";
		case BAD_VALUE_size:
			return "No hay elementos listados";
		default:
			return "Error desconocido";
	}
}





std::random_device Chromosome::rd;
std::mt19937 Chromosome::gen(rd());
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











/*
std::uniform_int_distribution<int> Junction::randChild(1,10);
std::uniform_int_distribution<int> Junction::randAlg(1,10);
std::uniform_int_distribution<int> Junction::randN(1,9);

Junction::Junction(const Junction& obj): Chromosome("Junction")
{
	number = obj.number;
	algorit = obj.algorit;
	type = obj.type;//backward compatible
}
Junction::Junction(): Chromosome("Junction")
{
	number = randChild(gen);
	algorit = randAlgt();
	type = TypeJuntion::BINARY;//backward compatible
}
Junction::Junction(unsigned short max): Chromosome("Junction")
{
	number = max;
	algorit = randAlgt();
	type = TypeJuntion::BINARY;//backward compatible
}
Junction::Junction(geneUS n,geneUS a): Chromosome("Junction")
{
	number = randChild(gen);
	algorit = randAlgt();
	type = TypeJuntion::BINARY;//backward compatible
}
Junction::Junction(TypeJuntion t): Chromosome("Junction")
{
	number = randChild(gen);
	algorit = randAlgt();
	type = t;//backward compatible
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
geneUS Junction::get_type()const
{
	return type;
}

void Junction::combine(const Chromosome& P1,const Chromosome& P2)
{
	copy(P1);
}

void Junction::copy(const Chromosome& P)
{
	number = ((Junction&)P).number;
	algorit = ((Junction&)P).algorit;
	type = ((Junction&)P).type;
}

void Junction::mutate(float p)
{
	double numrd1 = randNumber(0.0,1.0);
	if(numrd1 <= p) number = randNumber(1.0,10.0);
	numrd1 = randNumber(0.0,1.0);
	if(numrd1 <= p) algorit = randAlgt();
	//if(numrd1 <= p) maxChilds = randNumber(1.0,10.0);
}
geneUS Junction::randAlgt()
{
	double randNum = randNumber();
	if(randNum < 0.01) return COPY;

	return COMBINE;
}
geneUS Junction::randType()
{
	double randNum = randNumber(0.0,1.0)/2.0;
	if(randNum < 0.5) return TypeJuntion::UNARY;

	return TypeJuntion::BINARY;
}
geneUS Junction::randChild()
{
	return randNumber(1.0, maxChilds);
}
void Junction::randFill(bool favor)
{
	number = randChild(gen);
	algorit = randAlgt();
	if(type == TypeJuntion::NOT_TYPE) type = TypeJuntion::BINARY;//backward compatible
}
void Junction::randFill(TypeJuntion t)
{
	number = randChild(gen);
	algorit = randAlgt();
	type = t;//backward compatible
}
*/





	Save::Save()
	{
		out = NULL;
		new_leader = false;
	}
	Save::Save(std::ofstream& o)
	{
		out = &o;
	}
	Save::Save(std::ofstream* o)
	{
		out = o;
	}
	/*
	Save::operator std::ofstream&()
	{
		return *out;
	}
	std::ofstream& Save::operator =(std::ofstream& o)
	{
		out = &o;
		return o;
	}
	std::ofstream* Save::operator =(std::ofstream* o)
	{
		out = o;
		return o;
	}
	*/
	bool Save::getNewLeader()const
	{
		return new_leader;
	}





	SaveCollection::SaveCollection() : Save(NULL)
	{
	}
	SaveCollection::SaveCollection(const std::filesystem::path& dir) : directory(dir)
	{
	}
	SaveCollection::~SaveCollection()
	{
		if(out) close();
	}

	void SaveCollection::open(const std::filesystem::path& filename)
	{
		if(out) throw oct::core::Exception("Aun existe el archivo de iteracion anterior",__FILE__,__LINE__);

		std::filesystem::path strfn = directory / filename;
		out = new std::ofstream(strfn);
		if(not out->is_open())
		{
			std::string msg = "Fallo apertura de archivo '";
			msg = strfn.string() + "'";
			throw oct::core::Exception(msg,__FILE__,__LINE__);
		}
	}
	void SaveCollection::open(const std::filesystem::path& fn,const std::filesystem::path& dir)
	{
		if(out) throw oct::core::Exception("Aun existe el archivo de iteracion anterior",__FILE__,__LINE__);

		std::filesystem::path strfn = dir / fn;
		out = new std::ofstream(strfn);
		if(not out->is_open())
		{
			std::string msg = "Fallo apertura de archivo '";
			msg = strfn.string() + "'";
			throw oct::core::Exception(msg,__FILE__,__LINE__);
		}
	}
	void SaveCollection::close()
	{
		if(out)
		{
			out->flush();
			out->close();
			delete out;
			out = NULL;
		}
	}





	SaveCollectionByIteration::SaveCollectionByIteration(const std::filesystem::path& dir,const std::string& p) : SaveCollection(dir),prefix(p)
	{
	}

	void SaveCollectionByIteration::open(Iteration it)
	{
		if(out) throw oct::core::Exception("Aun existe el archivo de iteracion anterior",__FILE__,__LINE__);

		std::filesystem::path strfn = prefix + "-" + std::to_string(it) + ".csv";
		SaveCollection::open(strfn);
	}






	SaveIteration::SaveIteration(const std::filesystem::path& dir) : SaveCollectionByIteration(dir,"iterations")
	{
	}






	SaveChilds::SaveChilds(const std::filesystem::path& dir) : SaveCollectionByIteration(dir,"childs")
	{
	}





	SaveSelections::SaveSelections(const std::filesystem::path& dir) : SaveCollectionByIteration(dir,"selections")
	{
	}






	SaveSolutions::SaveSolutions()
	{
	}
	SaveSolutions::SaveSolutions(const std::filesystem::path& dir) : SaveCollection(dir)
	{
	}





std::random_device Single::rd;
std::mt19937 Single::gen(rd());
Single::Single(const Single& obj)
{
	env = obj.env;
	id = env->nextID();
	age = 0;
	fitness = obj.fitness;
	childs = obj.childs;
}
Single::Single(Enviroment& e) : env(&e)
{
	id = env->nextID();
	init();
	std::uniform_int_distribution<> distr(1, 9);
	childs = distr(gen);
}
Single::Single(Enviroment& e,unsigned int c) : childs(c),env(&e)
{
	id = env->nextID();
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
/*const Junction& Single::getJunction()const
{
	return junction;
}*/
unsigned int Single::getChilds()const
{
	return childs;
}
Enviroment& Single::getEnviroment()const
{
	return *env;
}

/*bool Single::mutation()const
{
	double numrand = randNumber(0.0,1.0);
	if(numrand <= env->getProbabilityMutationEvent()) return true;
	else return false;
}*/
void Single::deltaAge()
{
	age++;
}
void Single::init()
{
	age = 0;
	fitness = 0;
}




unsigned int table_reglog::getPredictX(real y)const
{
	return std::exp((y - b)/a);
}
unsigned int table_reglog::getPredictFinally() const
{
	return getPredictX(1.0);
}


std::random_device Enviroment::rd;
std::mt19937 Enviroment::gen(rd());

void Enviroment::init()
{
	idCount = 1;
	actualIteration = 0;
	maxIteration = 0;
	echolevel = 0;
	echoPrecision = 20;
	//logDirectoryFlag = false;
	//sigmaReduction = 1.0;
	//minSolutions = 0;
	//stopMinSolutions = false;
	mutableProb = 0.002;
	//pMutableGene = -1.0;
	//fout = NULL;
	stopMaxIterations=false;
	stopMaxSerie = false;
	//percen_at_iteration = 0.4;//%
	comparer = &cmpStrength;
	echoSteps = false;
	maxSerie = 0;
	epsilon = 1.0e-38;//para float y double
	maxPopulation = 0;
	maxProgenitor = 0;
	initPopulation = 0;
	echolevel = 0;
	sigma = 0;
	media = 0;
	actualSerie = 0;
	newIteration = false;
	echoF = NULL;
	savingDevice = NULL;
	//junting_sigma = 1.0;
	running = false;
	maxMutation = 0;

	/*prediction = true;
	prediction_table.sum_ln_x = 0;
	prediction_table.sum_ln2_x = 0;
	prediction_table.sum_ln_xy = 0;
	prediction_table.sum_y2 = 0;
	prediction_table.sum_ln_x_mean = 0;
	prediction_table.y_mean = 0;*/

	juntion_progenitor = NULL;
	//juntion_variety = NULL;
	//juntion_any = NULL;
	juntion_type = NULL;
}
void Enviroment::init2()
{
	if(maxProgenitor < 2) throw Exception(Exception::BAD_VALUE_maxProgenitor,__FILE__,__LINE__);
	if(size() == 0) throw Exception(Exception::BAD_VALUE_size,__FILE__,__LINE__);

	juntion_progenitor = new std::uniform_int_distribution<int>(1,maxProgenitor - 1);
	//juntion_variety = new std::uniform_int_distribution<int>(maxProgenitor - 1, maxPopulation - 1);
	//juntion_any = new std::uniform_int_distribution<int>(0, maxPopulation - 1);
	juntion_type = new std::uniform_int_distribution<int>(1, 3);

}
Enviroment::Enviroment()
{
	init();
}
Enviroment::~Enviroment()
{
	if(size() > 0) free();
	if(savingDevice) delete savingDevice;
	if(juntion_progenitor) delete juntion_progenitor;
	//if(juntion_variety) delete juntion_variety;
	//if(juntion_any) delete juntion_any;
	//if(juntion_any) delete juntion_type;
	//std::cout << "\t\tEnviroment::~Enviroment\n";
}

Enviroment::Enviroment(Iteration m) : maxIteration(m)
{
	init();
}
Enviroment::Enviroment(Iteration m,Iteration ms) : maxIteration(m),maxSerie(ms)
{
	init();
}
Enviroment::Enviroment(const std::filesystem::path& logDir,bool subtree)
{
	init();
	if(subtree) logDirectory = logDir / std::to_string(oct::core::getDayID());
	else logDirectory = logDir;
}
Enviroment::Enviroment(const std::filesystem::path& logDir,Iteration m) : maxIteration(m)
{
	if(not std::filesystem::exists(logDir))
	{
		std::string msg = "El directorio '";
		msg += logDir.string() + "' no existe.";
		throw oct::core::Exception(msg,__FILE__,__LINE__);
	}
	init();
	logDirectory = logDir;
	//if(not shell.exists(logDirectory)) shell.mkdir(logDirectory,true);
}
Enviroment::Enviroment(const std::filesystem::path& logDir,Iteration mi,Iteration ms) : maxIteration(mi),maxSerie(ms)
{
	if(not std::filesystem::exists(logDir))
	{
		std::string msg = "El directorio '";
		msg += logDir.string() + "' no existe.";
		throw oct::core::Exception(msg,__FILE__,__LINE__);
	}
	init();
	logDirectory = logDir / std::filesystem::path("serie-" + std::to_string(oct::core::getDayID()));//para iteracion
}
Enviroment::Enviroment(int argc, const char* argv[])
{
	init();
	commands(argc,argv);
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
double Enviroment::Enviroment::getMean() const
{
	return media;
}
double Enviroment::getEpsilon() const
{
	return epsilon;
}
/*double Enviroment::getProbabilityMutableGene()const
{
	return pMutableGene;
}*/
double Enviroment::getMutableProbability()const
{
	return mutableProb;
}
/*
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
unsigned int  Enviroment::getPredictFinally() const
{
	return prediction_table.getPredictFinally();
}*/
ID Enviroment::getCountID()
{
	return idCount;
}
echo Enviroment::getFout()
{
	return echoF;
}
unsigned short Enviroment::getEchoLevel()const
{
	return echolevel;
}
/*const std::string Enviroment::getLogSubDirectory()const
{
	return logSubDirectory;
}*/
bool Enviroment::getEchoSteps()const
{
	return echoSteps;
}
const std::filesystem::path& Enviroment::getLogDirectory()const
{
	return logDirectory;
}
const std::filesystem::path& Enviroment::getLogDirectoryHistory()const
{
	return logDirectoryHistory;
}
const std::filesystem::path& Enviroment::getLogDirectorySolutions()const
{
	return logDirectorySolutions;
}
Population Enviroment::get_population_size()const
{
	return size();
}
const ec::Single* Enviroment::get_single(Population i)const
{
	if(size() < i) return NULL;

	if(i == 0) return front();
	else if(i == 1)
	{
		const_iterator it = begin();
		it++;
		return *it;
	}
	else
	{
		const_iterator it = begin();
		std::advance(it,i);
		return *it;
	}
}

ID Enviroment::nextID()
{
	return ++idCount;
}
void Enviroment::enableEcho(echo f, unsigned short level)
{
	echoF = f;
	echolevel = level;
}
/*void Enviroment::enableLogFile(bool log)
{
	logFile = log;
}*/

const std::list<ec::Single*>& Enviroment::getSolutions()const
{
	return solutions;
}


bool Enviroment::run()
{
	if(stopMinSolutions and minSolutions == 0) throw octetos::core::Exception("La cantida minima de soluciones requeridad deve ser mayor que 0",__FILE__,__LINE__);
	if(initPopulation == 0) throw oct::core::Exception("La poblacion inicial deve ser mayor que 0",__FILE__,__LINE__);
	if(maxProgenitor > maxPopulation) throw oct::core::Exception("La catidad de progenitores deve ser menor que la popblacion",__FILE__,__LINE__);
	if(maxProgenitor == 0) throw oct::core::Exception("La cantiad de progenitore deve er mayor que 0",__FILE__,__LINE__);
	if(maxMutation == 0) throw oct::core::Exception("La cantiad de Mutacion deve ser mayor que 0",__FILE__,__LINE__);
	//if(gamma < 9.0e-38) throw oct::core::Exception("Asigne el valor gamma",__FILE__,__LINE__);

	if(actualIteration == 0) actualIteration = 1;
	//std::cout << "\tEnviroment::run : Step 1\n";
	if(size() == 0) initial();
	init2();
    //std::cout << "\tEnviroment::run : Step 2\n";
	for(ec::Single* single : *this)
	{
		single->eval();
	}
	//unsigned short counUndelete = 0;
	std::ofstream history;
	//std::cout << "\tEnviroment::run : Step 3\n";
	//std::cout << "logDirectory = " << logDirectory << "\n";
	//std::cout << "logDirectoryHistory  = " << logDirectoryHistory  << "\n";
	//std::cout << "logDirectorySolutions = " << logDirectorySolutions << "\n";
	logDirectoryFlag = not logDirectory.empty();
	if(logDirectoryFlag)
	{
		if(not std::filesystem::exists(logDirectory)) std::filesystem::create_directory(logDirectory);
		if(not std::filesystem::exists(logDirectory))
        {
            std::string msg = "No existe el directorio de logs '";
            msg += logDirectory.string() + "'";
            throw core::Exception(msg,__FILE__,__LINE__);
        }
		std::filesystem::path strhistory = logDirectory / "historial.csv";
		history.open(strhistory);
		logDirectoryHistory = logDirectory;
		logDirectorySolutions = logDirectory;
		logDirectoryHistoryFlag = true;
		logDirectorySolutionsFlag = true;
	}
	else
	{
		//std::cout << "\tStep 3.0\n";
		logDirectoryHistoryFlag = not logDirectoryHistory.empty();
		//std::cout << "\tStep 3.1\n";
		if(not std::filesystem::exists(logDirectoryHistory)) std::filesystem::create_directories(logDirectoryHistory);
		logDirectorySolutionsFlag = not logDirectorySolutions.empty();
		if(not std::filesystem::exists(logDirectorySolutions)) std::filesystem::create_directories(logDirectorySolutions);
		if(logDirectoryHistoryFlag)
		{
			std::filesystem::path strhistory = logDirectoryHistory / "historial.csv";
			history.open(strhistory);
		}
		//std::cout << "\tStep 3.2\n";
	}
	//std::cout << "\tStep 4\n";

	//ID oldleaderID = 0;
	//double oldLeaderFitness = 0.0;
	//Iteration countOldLeader = 0;
	//Iteration countOldLeaderFitness = 0;
	//std::mt19937 gen(rd());
	//Iteration maxRepeat = 10 + minSolutions;
	//bool triggerRepeatEnable = true;
	//double triggerRepeatMin = double(maxPopulation) * 1.0e-5;
	//double triggerJam2 = 1.0e-20;
	const ec::Single *leaderPrev, *leader;
	//std::cout << "mutableProb : " << mutableProb << "\n";
	std::bernoulli_distribution mutation_distr(mutableProb);
	std::uniform_int_distribution<int> distrib_maxm(1,maxMutation);
	//real fitness;
	if(echolevel > 0 and echoF != NULL) echoF("\n");
	bool ret = false;

	if(size() < 3) throw oct::core::Exception("Deve haber mas de dos individuos para ejecutar el programa",__FILE__,__LINE__);
	if(maxProgenitor  < 3) throw oct::core::Exception("Deve haber mas de dos individuos para ejecutar el programa",__FILE__,__LINE__);
	running = true;
	while(running)
	{
		if(stopMaxIterations)
		{
			if(actualIteration > maxIteration)
			{
				if(echoF) echoF("Se alacanzo el limite maximo de iteraciones\n");
				history.close();
				running = false;
				return false;
			}
		}
		if(echolevel > 0 and echoF != NULL)
		{
			if(maxSerie > 0 and stopMaxSerie)
			{
				std::string log = ">>> Serie : ";
				log += std::to_string(actualSerie) + "/" + std::to_string(maxSerie) + " - ";
				echoF(log.c_str());
			}
			echoF(">>> ");
			if(maxIteration > 0)
			{
				std::string log = "Iteracion : ";
				log += std::to_string(actualIteration) + "/" + std::to_string(maxIteration) + "\n";
				echoF(log.c_str());
			}
			else
			{
				std::string log = "Iteracion : ";
				log += std::to_string(actualIteration) + "\n";
				echoF(log.c_str());
			}
		}
		//std::cout << "\tEnviroment::run - while Step 1 , count : " << size() << "\n";

		if(not comparer)
		{
			throw oct::core::Exception("No se a asignado el creterio de coparacion.",__FILE__,__LINE__);
		}
		leaderPrev = front();
		sort(comparer);
		leader = front();
		//std::cout << "\tEnviroment::run - while Step 2 , count : " << size() << "\n";

		//media = oct::math::mean<ec::Single*,std::list,real>(*this,[](ec::Single* s)->real{return s->getFitness();});
		media = 0;
		sigma = 0;
		for(ec::Single* s : *this)
		{
			//std::cout << "\t" << s->getID() << " Adaptabilidad : " << s->getFitness() << "\n";
			if(s->getFitness() > 1 or s->getFitness() < 0)
			{
				std::string msg = "El fitness de cada individio deve estar en el intervalo [0,1], se encontro ";
				msg += std::to_string(s->getFitness());
				throw oct::core::Exception(msg,__FILE__,__LINE__);
			}
			media += s->getFitness();

			s->deltaAge();
		}
		media /= real(size());
		for(ec::Single* s : *this)
		{
			//std::cout << "\t" << s->getID() << " Fortaleza : " << s->getStrength() << "\n";
			sigma += pow(s->getFitness() - media,real(2));
		}
		sigma /= real(size());
		//sigma = oct::math::variation<ec::Single*,std::list,real>(*this,media,[](ec::Single* s)->real{return s->getFitness();});
		/*if(leaderPrev != leader)
		{//hay un nuevo lider
			//TODO:optener una promedio a partir de una progresion
			//https://es.wikipedia.org/wiki/Regresi%C3%B3n_no_lineal
			//Note:Regirion Logaritmica : pendiente en cuaderno
			if(prediction)
			{//x=actualIteration, y = leader->getFitness()
				prediction_table.sum_ln_x += log(real(actualIteration));
				prediction_table.sum_ln2_x += std::pow(prediction_table.sum_ln_x,real(2));
				prediction_table.sum_ln_xy += log(real(actualIteration)) * leader->getFitness();
				prediction_table.sum_y2 += std::pow(leader->getFitness(),real(2));
				prediction_table.b = prediction_table.a * prediction_table.sum_ln_x;//TODO:se deve usar la media del logaritmo
				prediction_table.sum_ln_x_mean = ((real(actualIteration - 1) * prediction_table.sum_ln_x_mean) + prediction_table.sum_ln_x)/real(actualIteration);
				prediction_table.y_mean = ((real(actualIteration - 1) * prediction_table.y_mean) + prediction_table.y_mean)/real(actualIteration);

				real a1 = prediction_table.sum_ln_xy - (prediction_table.y_mean * prediction_table.sum_ln_x);
				real a2 = prediction_table.sum_ln2_x - (prediction_table.sum_ln_x_mean * prediction_table.sum_ln_x);
				prediction_table.a = a1 / a2;
			}
		}*/
		//std::cout << "\tEnviroment::run - while Step 3 , count : " << size() << "\n";
		if(logDirectoryFlag)
		{
		    //std::cout << "\tEnviroment::run - while Step 3.1\n";
			SaveIteration saveit(logDirectory);
			//std::cout << "\tEnviroment::run - while Step 3.2\n";
			saveit.open(actualIteration);
			//std::cout << "\tEnviroment::run - while Step 3.3\n";
			if(leaderPrev != leader) saveit.new_leader = true;
			else saveit.new_leader = false;
			//std::cout << "\tEnviroment::run - while Step 3.4\n";
			for(ec::Single* s : *this)
			{
			    //std::cout << "\tEnviroment::run - while Step 3.4.1\n";
				s->save(saveit);
				//std::cout << "\tEnviroment::run - while Step 3.4.2\n";
				(*saveit.out) << "\n";
				//std::cout << "\tEnviroment::run - while Step 3.4.3\n";
			}
			//std::cout << "\tEnviroment::run - while Step 3.5\n";
			saveit.close();
			//std::cout << "\tEnviroment::run - while Step 3.6\n";
		}
		//std::cout << "\tEnviroment::run - while Step 3.7\n";
		std::time_t t;
		tm time;
		if(logDirectoryHistoryFlag)
		{
			if(history.is_open())
			{
				t = std::time(nullptr);
				time = *std::localtime(&t);

				history  << std::put_time(&time,"%d/%m/%Y %H:%M:%S");
				history  << ",";
				history  << actualIteration;
				history  << ",";
				history  << maxPopulation;
				history  << ",";
				history  << maxProgenitor;
				history  << ",";
				history  << std::setprecision(echoPrecision) << media;
				history  << ",";
				history  << std::setprecision(echoPrecision) << sigma;
				history  << ",";
				history  << std::setprecision(echoPrecision) << (size() > 0 ? (*front()).getFitness() : 0);//fitness lider
				history  << ",";
				history  << mutableProb;
				history  << "\n";

				history.flush();
			}
		}
		solutions.clear();
		for(iterator it = begin(); it != end(); it++)
		{
			if( 1.0 - (*it)->getFitness() < epsilon) solutions.push_back(*it);
			else break;//si no fuen solucio las siguientes tampoco
		}
		if(solutions.size() >= minSolutions and logDirectorySolutionsFlag and stopMinSolutions)//se definion una cantidad minima de soluciones
		{
			if(echolevel > 0 and echoF != NULL)
			{
				std::string log = "\n\tSe completo el conjunto de solucion minimo : ";
				log += std::to_string(solutions.size()) + "\n";
				echoF(log.c_str());
			}
			SaveSolutions saveSols;
			saveSols.open("solutions.cvs",logDirectorySolutions);
			for(Single* s : solutions)
			{
				s->save(saveSols);
				(*saveSols.out) << "\n";
			}
			saveSols.close();
			history.close();
			running = false;
			return true;
		}
		else if(solutions.size() == maxPopulation and logDirectorySolutionsFlag)//se definion una cantidad minima de soluciones
		{
			if(echolevel > 0 and echoF != NULL)
			{
				std::string log = "\n\tSe completo el conjunto de solucion minimo : ";
				log += std::to_string(solutions.size()) + "\n";
			}
			SaveSolutions saveSols;
			saveSols.open("solutions.cvs",logDirectorySolutions);
			for(Single* s : solutions)
			{
				s->save(saveSols);
				(*saveSols.out) << "\n";
			}
			saveSols.close();
			history.close();
			running = false;
			return true;
		}
		else if(solutions.size() >= minSolutions and stopMinSolutions)//se definion una cantidad minima de soluciones
		{
			if(echolevel > 0 and echoF != NULL)
			{
				std::string log = "\n\tSe completo el conjunto de solucion minimo : ";
				log += std::to_string(solutions.size()) + "\n";
			}
            running = false;
			return true;
		}
		else if(solutions.size() == maxPopulation)//se definion una cantidad minima de soluciones
		{
			if(echolevel > 0 and echoF != NULL)
			{
				std::string log = "\n\tSe completo el conjunto de solucion minimo : ";
				log += std::to_string(solutions.size()) + "\n";
			}
			running = false;
			return true;
		}
        //std::cout << "\tEnviroment::run - while Step 4 , count : " << size() << "\n";
		ec::ID countBefore = size();
		selection();
		if(logDirectoryFlag)
		{
			SaveSelections saveSelections(logDirectory);
			saveSelections.open(actualIteration);

			if(logDirectoryFlag)
			{
				for(Single* s : *this)
				{
					s->save(saveSelections);
					(*saveSelections.out) << "\n";
				}
			}
			saveSelections.close();
		}
		//std::cout << "\tEnviroment::run - while Step 5 , count : " << size() << "\n";
		unsigned short removes = countBefore - size();
		//deletes == 0 ? counUndelete++ : counUndelete = 0;
		if(echolevel > 1 and echoF != NULL)
		{
			std::string log = "\tProgenitores selecionados, total : ";
			log += std::to_string(size()) + "\n";
			log += "\tEliminados : ";
			log += std::to_string(removes) + "\n";
			//if(prediction) log += "\tPrediccion : " + std::to_string(prediction_table.getPredictFinally()) + "\n";
			echoF(log.c_str());
		}
		//std::cout << "\tEnviroment::run - while Step 6 , count : " << size() << "\n";


		//std::cout << "\tStep C10\n";
		if(echolevel > 1 and echoF != NULL)
		{
		    std::stringstream stream;
		    stream.precision(echoPrecision);
		    stream << std::fixed;
		    stream << leader->getFitness();
			std::string log = "\tLider : ";
			log += stream.str() + "\n";
			log += "\tDesviacion estandar : ";
			stream.clear();
			stream.str("");
			stream << sigma;
			log += stream.str() + "\n";
			echoF(log.c_str());
		}

		//std::cout << "\tEnviroment::run - while Step 7 , count : " << size() << "\n";

		juncting();
		SaveChilds savechilds(logDirectory);
		//std::cout << "\tEnviroment::run - while Step 8 , count : " << size() << "\n";
		if(logDirectoryFlag) savechilds.open(actualIteration);
		for(ec::Single* s : newschils)//agregar los nuevos hijos a la poblacion
		{
		    if(mutation_distr(rd))
		    {
                unsigned int i = (maxMutation == 1)? i = 0 : i = distrib_maxm(gen);
                for(; i < maxMutation; i++) s->mutate();
			}
			s->eval();
			if(logDirectoryFlag)
			{
				s->save(savechilds);
				(*savechilds.out) << "\n";;
			}
			push_front(s);
		}
		if(logDirectoryFlag) savechilds.close();
		if(echolevel > 1 and echoF != NULL)
		{
			std::string log = "\tNuevos Hijos : ";
			log += std::to_string(newschils.size()) + "\n";
			echoF(log.c_str());
		}
		newschils.clear();
		//std::cout << "\tEnviroment::run - while Step 9 , count : " << size() << "\n";

		actualIteration++;
	}
	running = false;
	return false;
}
bool Enviroment::run(int argc, const char* argv[])
{
	commands(argc,argv);
	return run();
}
bool Enviroment::series()
{
	for(actualSerie = 1 ; actualSerie <= maxSerie ; actualSerie++)
	{
		free();

		if(run()) return true;
	}

	return false;
}
bool Enviroment::series(int argc, const char* argv[])
{
	commands(argc,argv);

	for(actualSerie = 1 ; actualSerie <= maxSerie ; actualSerie++)
	{
		free();

		if(run()) return true;
	}

	return false;
}
void Enviroment::stopperMaxIterations(Iteration max)
{
	maxIteration = max;
	stopMaxIterations = true;
}
/*void Enviroment::stopperNotDiference(double cota)
{
	notDiferenceCota = cota;
	stopNotDiference = true;
}*/
void Enviroment::stopperMinSolutions(Population min)
{
	minSolutions = min;
	stopMinSolutions = true;
}
void Enviroment::stopperMaxSerie(Iteration max)
{
	maxSerie = max;
	stopMaxSerie = true;
}


ec::Single* Enviroment::getProxSolution()
{
	ec::Single* s;
	for(iterator it = begin(); it != end(); it++)
	{
		s = *it;
		if(1.0 - s->getFitness() < epsilon) return s;
	}

	return *begin();
}

Single* Enviroment::getRandomSingleAny()
{
	iterator it = begin();
	std::advance(it,juntion_progenitor->operator()(gen));

	return *it;
}
Single* Enviroment::getRandomSingleFirst()
{
	return front();
}
Single* Enviroment::getRandomSingleSecond()
{
	iterator it = begin();
	it++;
	return *it;
}

Single* Enviroment::getRandomSingleRandom()
{
	int type = juntion_type->operator()(gen);
	switch(type)
	{
		case 1:
			return getRandomSingleSecond();
		case 2:
			return getRandomSingleAny();
		default:
			return getRandomSingleAny();
	}

	return getRandomSingleAny();
}
Iteration Enviroment::getIterationActual()const
{
	return actualIteration;
}
void Enviroment::juncting()
{
	Single *single1,*single2;
	do
	{
		single1 = getRandomSingleFirst();
		do
		{
			single2 = getRandomSingleAny();
		}
		while(single1 == single2);//repeat until diferent

		single1->juncting(newschils,single2);
	}
	while(newschils.size() + size() <= maxPopulation);
}
void Enviroment::setEchoSteps(bool e)
{
	echoSteps = e;
}
/*
void Enviroment::eval()
{
	Single* single;
	for(ec::Single* s : *this)
	{
		single = (Single*) s;
		single->eval();
	}
}
*/
void Enviroment::save()
{
	std::filesystem::path strfn = logDirectory / std::filesystem::path("solutions-" + std::to_string(actualIteration) + ".csv");
	std::ofstream fn(strfn);
	for(ec::Single* s : *this)
	{
		s->save(*savingDevice);
		fn << "\n";
	}
	fn.flush();
	fn.close();
}

/**
*\brief Elimina los que no cumple con el criterio de seleccion
*/
void Enviroment::selection()
{//TODO:se puede mejorar al comenzar a eliminar desde el final hasta maxProgenitor
	//liberar memorio en el indicie porteror a maxProgenitor hasta el final de la lista
	if(size() < maxProgenitor) throw oct::core::Exception("El tama�o de la pobacion es menor que la cantidad de progenitores",__FILE__,__LINE__);
	std::list<ec::Single*>::iterator it = begin();
	std::advance(it,maxProgenitor); // es n + 1
	while(it != end())
	{
		delete *it;

		//
		it++;
	}
	//eliminar los elemenbtos sobrantes de la lista
	resize(maxProgenitor);
}

void Enviroment::save(const std::list<ec::Single*>& lst, const std::filesystem::path& file)
{
	std::filesystem::path strfn = logDirectorySolutions / file;
	std::ofstream fn(strfn);
	for(ec::Single* s : lst)
	{
		s->save(*savingDevice);
		fn << "\n";
	}
	fn.flush();
	fn.close();
}

void Enviroment::commands(int argc, const char* argv[])
{
	//std::cout << "Enviroment::commands : Step 1.0\n";
	for(int i = 1; i < argc; i++)
	{
		if(strcmp("--directory-logs",argv[i]) == 0)
		{
			if(i + 1 > argc) throw core::Exception("No se agrego el parametro solicitado",__FILE__,__LINE__);
			logDirectory = argv[++i];
			basedir = logDirectory;
		}
		else if(strcmp("--directory-history-logs",argv[i]) == 0)
		{
			if(i + 1 > argc) throw core::Exception("No se agrego el parametro solicitado",__FILE__,__LINE__);
			logDirectoryHistory = argv[++i];
		}
		else if(strcmp("--directory-solutions",argv[i]) == 0)
		{
			if(i + 1 > argc) throw core::Exception("No se agrego el parametro solicitado",__FILE__,__LINE__);
			logDirectorySolutions= argv[++i];
		}
		else if(strcmp("--iterations",argv[i]) == 0)
		{
			if(i + 1 > argc) throw core::Exception("No se agrego el parametro solicitado",__FILE__,__LINE__);
			stopperMaxIterations(std::stoi(argv[++i]));
		}
		else if(strcmp("--serie",argv[i]) == 0)
		{
			if(logDirectory.empty()) throw oct::core::Exception("Asigne primero el directorio de ejecucion",__FILE__,__LINE__);

			//serieName = argv[++i];
			std::string strDay = std::to_string(oct::core::getDayID());
			logDirectory = logDirectory / strDay;
			if(i + 1 < argc) throw core::Exception("No se agrego el parametro solicitado",__FILE__,__LINE__);
			stopperMaxSerie(std::stoi(argv[++i]));
			//std::cout << "serie = " << argv[i] << "\n";
		}
		else if(strcmp("--max-treat",argv[i]) == 0)
		{
		}
		else if(strcmp("--mutation-probability",argv[i]) == 0)
		{
			if(i + 1 > argc) throw core::Exception("No se agrego el parametro solicitado",__FILE__,__LINE__);
			mutableProb = std::stod(argv[++i]);
		}
		else if(strcmp("--create-session",argv[i]) == 0)
		{
            create_session();
		}
		else if(strcmp("--solutions",argv[i]) == 0)
		{
			if(i + 1 > argc) throw core::Exception("No se agrego el parametro solicitado",__FILE__,__LINE__);
			minSolutions = std::stoi(argv[++i]);
		}
		/*else
		{
			std::cout << "Opcion desconocida : " << argv[i] << "\n";
		}*/
	}
}
void Enviroment::create_session()
{
    std::string strDay = std::to_string(oct::core::getDayID());
    std::string strTime = std::to_string(oct::core::getTimeID());
    if(not logDirectory.empty())
    {
        logDirectory = logDirectory / strDay / strTime;
    }
    else
    {
        if(not logDirectoryHistory.empty())  logDirectoryHistory = logDirectoryHistory /strDay / strTime;
        if(not logDirectorySolutions.empty())  logDirectorySolutions = logDirectorySolutions /strDay / strTime;
    }

}
void Enviroment::free()
{
	for(Single* s : *this)
	{
		delete s;
	}

	clear();
	//std::cout << "Enviroment::free\n";
}
void Enviroment::stop()
{
	running = false;
}
bool Enviroment::isRunning()
{
	return running;
}
bool Enviroment::getBetters(unsigned int count, std::list<ec::Single*>& list)
{
	if(count > size()) return false;
	iterator it = begin();

	for(unsigned int i = 0; i < count; i++)
	{
		list.push_back(*it);
		it++;
	}

	return true;
}
real Enviroment::getProgress()const
{
	if(size() < minSolutions) throw core::Exception("Esta solicitando mas elementos de los existentes",__FILE__,__LINE__);;

	if(minSolutions == 1) return front()->getFitness();

	real prog = 0;
	const_iterator it = begin();
	for(unsigned int i = 0; i < minSolutions; i++)
	{
		prog += (*it)->getFitness();
		it++;
	}
	prog /= real(minSolutions);
	return prog;
}
}
