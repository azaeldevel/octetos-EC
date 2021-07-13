

#ifndef AE_GA_EXT_HH
#define AE_GA_EXT_HH

#include <octetos/core/MD5sum.hh>

#include "GA.hh"


namespace ec::sudoku
{
class Enviroment;


class Chromosome : public ec::Chromosome
{
public:
	Chromosome();
	Chromosome(const Chromosome& obj);
	virtual ~Chromosome();
	geneUS getNumber(unsigned short i,unsigned short j) const;
	void setNumber(unsigned short i,unsigned short j,geneUS);

	const Chromosome& operator = (const Chromosome&);	
	
	virtual void combine(const ec::Chromosome& P1,const ec::Chromosome& P2);
	virtual void copy(const ec::Chromosome& P1);
	virtual void mutate(float p);
	virtual void randFill(bool favor = false);

	/*
	*\brief Busca un numero que no halla sido asignado.
	*/
	geneUS freeNumber()const;
	
	/*
	*\brief Detec colision
	*/
	void resetCollision();

	
private:
	geneUS numbers[3][3];
};

class Single : public ec::Single
{
public:
	Single(const Single&);
	Single(unsigned int id,Enviroment& e,Chromosome init[][3]);
	Single(unsigned int id,Enviroment& e,Chromosome** newData,Chromosome init[][3], const Junction& junction);
	virtual ~Single();

	const Chromosome& getTalba(unsigned short i,unsigned short j)const;
		
	virtual void eval();
	virtual void randFill(bool favor = false);
	virtual Population juncting(std::list<ec::Single*>& chils,const ec::Single* single,unsigned short loglevel,void*) const;
	virtual void save(std::ofstream& fn);
	const octetos::core::MD5sum& getMD5() const;
	
	void print(std::ostream&) const;
	void printInit(std::ostream&) const;
	unsigned int getErros()const;
	
private:
	Chromosome** tabla;
	Chromosome (*intiVals)[3];
	octetos::core::MD5sum md5;

	//
	void genMD5();
};

class Enviroment : public ec::Enviroment
{
public:
	//
	
	//
	Enviroment();	
	Enviroment(const std::string& initTable);
	Enviroment(const std::string& initTable,Iteration maxIteration);
	Enviroment(const std::string& initTable,Iteration maxIteration,const std::string& logDir);
	virtual ~Enviroment();
	void init(const std::string& initB);
	void initBoard(const std::string& initTable);

	unsigned short getFaltantes() const;
	double getGamma() const;
	//void saveSolutions(const std::string& dir)const;
	void saveSolutions(std::ofstream& f)const;
	
	virtual void selection();
	//virtual bool run();
	virtual void initial();
	virtual void eval();
	virtual void juncting();
	virtual void save();

private:

	/**
	*\brief valor estadistico de cada variable.
	*/
	double gamma;
	Chromosome sudokuInit[3][3];
	std::string fnBoard;
	
};

}

#endif