

#ifndef EC_SCHEDULE_HH
#define EC_SCHEDULE_HH

#include "GA.hh"


namespace oct::ec::sche
{
	
	class Enviroment;

	struct Time
	{
		tm begin;
		tm end;

		Time()
		{
			begin = {0};
			end = {0};
		}
	};

	class Target
	{
	public:
		virtual const std::string& get_name() = 0;
	};

	struct Grain
	{
		const Target* target;
		Time time;
	};
	

	/**
	*\brief Representa una posible solucion a nuestro problema
	*/
	class Single : public oct::ec::Single
	{
	public:
		/**
		*\brief Constructor que recive el ID del Individuo y el Ambiente
		*/
		Single(ID id,Enviroment& env);
		
		/**
		*\brief Evalua al individuo y asigna su valor de adaptabilidad(fitness)
		*/
		virtual void eval();		
		/**
		*\brief Crea un arcivo CSV con los datos relevantes del individuo
		*/
		virtual void save(std::ofstream& fn);		
		/**
		*\brief Realiza el apareo entre dos individuos
		*/
		virtual Population juncting(std::list<oct::ec::Single*>& chils,const oct::ec::Single* single,unsigned short loglevel,void*);
		/**
		*\brief Imprime los datos relevantes del individuo
		*/
		virtual void print(std::ostream&) const;
	private:
		
	};

	/**
	*\brief Variables de control y proceso
	*/
	class Enviroment : public oct::ec::Enviroment
	{
	public:
		/**
		*\brief Constructor que solomante llama a init
		*/
		Enviroment();
		/**
		*\brief Constructor que recive un directorio para guardar estadisticas
		*/
		Enviroment(const std::string& log);
		/**
		*\brief Inicia las variables
		*/
		void init();
		~Enviroment();

	
		/**
		*\brief Crea la poblacion inicial
		*/
		virtual void initial();
	private:

	};

}

#endif