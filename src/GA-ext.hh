

#ifndef AE_GA_EXT_HH
#define AE_GA_EXT_HH

#include <octetos/core/MD5sum.hh>

#include "GA.hh"


namespace ae::ga
{

/*
class MaxMin_R : public Chromosome
{
public:
	MaxMin_R();
	MaxMin_R(geneF x2_mx,geneF x2_mn);
	geneF get_x2_mx()const;
	geneF get_x2_mn()const;
	virtual void combine(const Chromosome& P1,const Chromosome& P2);
	virtual void copycombine(const Chromosome& P1,const Chromosome& P2);
	virtual void copy(const Chromosome& P1,const Chromosome& P2);
	virtual void mutate();

private:
	geneF x2_mx;
	geneF x2_mn;
};


struct Notas
{
	float x2_max;//la tendecia hacia 0 es mejor
	float x2_min;//la tendecia hacia 0 es mejor

	float strength;
};
class MaxMin_x2 : public Single
{
public:
	MaxMin_x2(unsigned int id,geneF x2_mx,geneF x2_min,geneUS number,geneUS algorit);
	MaxMin_x2(unsigned int id,MaxMin_R mm,const Junction& j);

public:
	MaxMin_R& getMaxMin();

	virtual void junction(MaxMin_x2& single,std::vector<MaxMin_x2*>& chils,unsigned short& count);

	static void junction(std::vector<ae::ga::MaxMin_x2*>& p,std::map<ae::ga::MaxMin_x2*,Notas>& e,unsigned short& idCount);
	static void eval_x2(const std::vector<ae::ga::MaxMin_x2*>& population,std::map<ae::ga::MaxMin_x2*,Notas>& evaluation);
	static void saveEvals(const std::string& prefixfn, const std::map<ae::ga::MaxMin_x2*,Notas>& result, unsigned short iteration);
	static int main(int argc, const char* argv[]);
private:
	MaxMin_R maxmin;
	//Junction juntion;
};
struct MaxMin_By_Value
{
	ae::ga::MaxMin_x2* maxmin;
	float value;
};
*/


class SudokuChromosome : public Chromosome
{
public:
	SudokuChromosome();
	SudokuChromosome(const SudokuChromosome& obj);
	geneUS getNumber(unsigned short i,unsigned short j) const;

	const SudokuChromosome& operator = (const SudokuChromosome&);
	
	geneUS& number(unsigned short i,unsigned short j);
	virtual void combine(const ae::Chromosome& P1,const ae::Chromosome& P2);
	virtual void copy(const ae::Chromosome& P1,const ae::Chromosome& P2);
	virtual void mutate(float p);
	virtual void randFill();
	
private:
	geneUS numbers[3][3];
};

class SudokuSingle : public Single
{
public:
	SudokuSingle(unsigned int id,const Enviroment& e,const SudokuChromosome (*t)[3]);
	SudokuSingle(unsigned int id,const Enviroment& e,const SudokuChromosome (*t)[3], const Junction& junction);
	const SudokuChromosome& getTalba(unsigned short i,unsigned short j)const;
		
	virtual void eval();
	virtual void randFill();
	virtual void juncting(ID& idCount,std::list<ae::Single*>& chils,ae::Single* single,unsigned short loglevel);
	virtual void saveEval(std::ofstream& fn);
	const octetos::core::MD5sum& getMD5() const;
	void genMD5();
	void print(std::ostream&) const;
	
private:
	SudokuChromosome tabla[3][3];
	const SudokuChromosome (*intiVals)[3];
	octetos::core::MD5sum md5;
};

class SudokuEnviroment : public Enviroment
{
public:
	SudokuEnviroment();

	unsigned short getFaltantes() const;
	double getGamma() const;
	ae::Single* getRandomSingleTop()const;
	ae::Single* getRandomSingle()const;
	
	virtual void run();
private:

	/**
	*\brief valor stadistico de cada variable.
	*/
	double gamma;
};

}

#endif