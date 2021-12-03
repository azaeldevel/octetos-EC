/*
 * main.cc
 * Copyright (C) 2021 Azael Reyes <azael.devel@gmail.com>
 * 
 * octetos-schedule is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * octetos-schedule is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sstream>
#include <fstream>
#include <iostream>
#include <locale>
#include <ctime>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "schedule.hh"

#ifdef _WIN32

extern "C" char* strptime(const char* s,const char* f,struct tm* tm) 
{
	std::istringstream input(s);
	input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
	input >> std::get_time(tm, f);
	if (input.fail()) {
		return nullptr;
	}
	return (char*)(s + input.tellg());
}

#endif

namespace oct::core
{
	Person::Person(const std::string& n,const std::string& ap,const std::string& am)
	{
		name = n + " " + ap + " " + am;
	}
	Person::Person(const std::string& n)
	{
		name = n;
	}
	Person::Person()
	{
	}
	 
	const std::string& Person::operator =(const std::string& n)
	{
		name = n;
		
		return n;
	}
	const std::string& Person::get_name()const
	{
		return name;
	}

	DataTime::DataTime()
	{
		(tm&)*this = {0};
	}
	const time_t* DataTime::operator =(const time_t* t)
	{
		tm* thistm = localtime(t);
		(tm&)*this = *thistm;
		return t;
	}
	int DataTime::get_week_day()const
	{
		return tm_wday;
	}

	double DataTime::diff(const DataTime& dt)const
	{
		time_t tm1,tm2;
		tm t1 = *this;
		tm t2 = dt;
		tm1 = mktime(&t1);
		tm2 = mktime(&t2);
		return difftime(tm2, tm1);		
	}
}

namespace oct::ec::sche
{
	Configuration::Configuration()
	{
		schema_week = SchemaWeek::MS;
		time_per_hour = 60;
	}
	
	unsigned int Configuration::get_time_per_hour() const
	{
		return time_per_hour;
	}
	Configuration::SchemaWeek Configuration::get_schema_week()const
	{
		return schema_week;
	}
	
	long Configuration::to_hours(double t)const
	{
		long mins = t/60.0;
		long hours = mins/time_per_hour;
		return hours;
	}

	
	

	Teacher::Teacher(const std::string& name,const std::string& ap,const std::string& am) : oct::core::Person(name,ap,am)
	{
		
	}
	Teacher::Teacher(const std::string& name) : oct::core::Person(name)
	{
		
	}
	Teacher::Teacher()
	{
		
	}




	
	
	Room::Room(const std::string& n)
	{
		name = n;
	}
	Room::Room()
	{
		
	}
	Room& Room::operator =(const std::string& n)
	{
		name = n;
		
		return *this;
	}
	const std::string& Room::get_name()
	{
		return name;
	}


	
	Subject::Subject(const std::string& n)
	{
		name = n;
	}
	Subject::Subject()
	{
		
	}
	const std::string& Subject::get_name()
	{
		return name;
	}
	void Subject::set(const std::string& n,unsigned int t)
	{
		name = n;
		time = t;
	}
	unsigned int Subject::get_time()const
	{
		return time;
	}

	
	Teachers::Row::Row()
	{
	}
	Teachers::Row::Row(int z) : std::vector<ec::sche::Time>(z)
	{
	}
	void Teachers::Row::print(std::ostream& out) const
	{
		out << teacher.get_name() << ",";
		for(unsigned int i = 0; i < size(); i++)
		{
			out << std::put_time(&at(i).begin, "%H:%M");
			out << "-";
			out << std::put_time(&at(i).end, "%H:%M");
			if(i < size() - 1) out << ",";
		}
	}
	
	Teachers::Teachers(const std::string& fn)
	{
		loadFile(fn);
	}
	Teachers::Teachers()
	{

	}
	const std::list<Teachers::Row>& Teachers::get_list() const
	{
		return teachers;
	}
	void Teachers::loadFile(const std::string& fn)
	{
		std::fstream csv(fn, std::ios::in);
		std::string line,data,strTime,strH;
		if(csv.is_open())
		{
			while(std::getline(csv,line))
			{
				std::stringstream str(line);
				std::getline(str,data,',');
				Teachers::Row row;
				//std::cout << data << ",";
				row.teacher = data;
				ec::sche::Time time;
				while(std::getline(str,data,','))
				{
					std::stringstream ssTime(data);
					std::getline(ssTime,strH,'-');
					strptime(strH.c_str(), "%H:%M",&time.begin);
					std::getline(ssTime,strH,'-');
					strptime(strH.c_str(), "%H:%M",&time.end);
					/*
					std::cout << std::put_time(&time.begin, "%H:%M");
					std::cout << "-";
					std::cout << std::put_time(&time.end, "%H:%M");
					std::cout << ",";
					*/
					row.push_back(time);
				}
				teachers.push_back(row);	
				//std::cout << "\n";
			}
			indexing();
		}	
		else
		{
			std::string msg = "Fallo la aperturade '";
			msg += fn + "'";
			throw core::Exception(msg,__FILE__,__LINE__);
		}	
	}
	void Teachers::print(std::ostream& out)
	{
		for(Row& row : teachers)
		{
			row.print(out);
			out << "\n";
		}
	}	
	void Teachers::indexing()
	{
		if(teacher_by_name.size() > 0) teacher_by_name.clear();
		for(Row& row : teachers)
		{
			teacher_by_name.insert({row.teacher.get_name().c_str(),&row});
		}
	}
	const Teachers::Row* Teachers::search(const std::string& str) const
	{
		std::map<std::string, Row*>::const_iterator it = teacher_by_name.find(str);

		if(it != teacher_by_name.end()) return it->second;
		return NULL;		
	}




	

	
	
	Subjects::Row::Row()
	{
		
	}
	Subjects::Subjects(const std::string& fn)
	{
		loadFile(fn);
	}
	Subjects::Subjects()
	{

	}
	void Subjects::loadFile(const std::string& fn)
	{
		std::fstream csv(fn, std::ios::in);
		std::string line,data;
		if(csv.is_open())
		{
			while(std::getline(csv,line))
			{
				std::stringstream str(line);
				Subjects::Row row;
				//std::cout << line;
				//std::cout << data << ",";

				std::getline(str,data,',');
				//std::cout << data << ",";
				std::string name = data;	
				
				std::getline(str,data,',');
				std::string time = data;
				//std::cout << "\n";
				row.subject.set(name,std::stoi(time));
				rooms.push_back(row);
			}
		}
		else
		{
			std::string msg = "Fallo la aperturade '";
			msg += fn + "'";
			throw core::Exception(msg,__FILE__,__LINE__);
		}		
	}
	void Subjects::print(std::ostream& out)
	{
		for(Row& row : rooms)
		{
			out << row.subject.get_name() << ",";
			out << row.subject.get_time();
			out << "\n";
		}
	}





	Teachers_Subjects::Row::Row()
	{
		
	}
	Teachers_Subjects::Teachers_Subjects(const std::string& fn)
	{
		loadFile(fn);
	}
	Teachers_Subjects::Teachers_Subjects()
	{

	}
	void Teachers_Subjects::loadFile(const std::string& fn)
	{
		std::fstream csv(fn, std::ios::in);
		std::string line,data;
		if(csv.is_open())
		{
			while(std::getline(csv,line))
			{
				std::stringstream str(line);
				//std::cout << line;
				std::getline(str,data,',');
				Teachers_Subjects::Row row;
				row.teacher = data;
				//std::cout << data << ",";

				std::getline(str,data,',');
				//std::cout << data << ",";
				row.subject = data;				
				//std::cout << "\n";
				rooms.push_back(row);
			}
		}	
		else
		{
			std::string msg = "Fallo la aperturade '";
			msg += fn + "'";
			throw core::Exception(msg,__FILE__,__LINE__);
		}
	}
	void Teachers_Subjects::print(std::ostream& out)
	{
		for(Row& row : rooms)
		{
			out << row.teacher.get_name() << ",";
			out << row.subject.get_name();
			out << "\n";
		}
	}

	
	
	Rooms::Row::Row()
	{
	}
	Rooms::Row::Row(int z) : std::vector<ec::sche::Time>(z)
	{
	}
	Rooms::Rooms(const std::string& fn)
	{
		loadFile(fn);
	}
	Rooms::Rooms()
	{
		
	}
	void Rooms::loadFile(const std::string& fn)
	{
		std::fstream csv(fn, std::ios::in);
		std::string line,data,strTime,strH;
		if(csv.is_open())
		{
			while(std::getline(csv,line))
			{
				std::stringstream str(line);
				std::getline(str,data,',');
				Rooms::Row row;
				//std::cout << data << ",";
				row.room = data;
				std::getline(str,data,',');
				row.subject = data;
				ec::sche::Time time;
				while(std::getline(str,data,','))
				{
					std::stringstream ssTime(data);
					std::getline(ssTime,strH,'-');
					strptime(strH.c_str(), "%H:%M",&time.begin);
					std::getline(ssTime,strH,'-');
					strptime(strH.c_str(), "%H:%M",&time.end);
					/*
					std::cout << std::put_time(&time.begin, "%H:%M");
					std::cout << "-";
					std::cout << std::put_time(&time.end, "%H:%M");
					std::cout << ",";
					*/
					row.push_back(time);
				}
				rooms.push_back(row);	
				//std::cout << "\n";
			}
		}	
		else
		{
			std::string msg = "Fallo la aperturade '";
			msg += fn + "'";
			throw core::Exception(msg,__FILE__,__LINE__);
		}	
	}	
	void Rooms::print(std::ostream& out)
	{
		for(Row& row : rooms)
		{
			out << row.room.get_name() << ",";
			out << row.subject.get_name() << ",";
			for(unsigned int i = 0; i < row.size(); i++)
			{
				out << std::put_time(&row[i].begin, "%H:%M");
				out << "-";
				out << std::put_time(&row[i].end, "%H:%M");
				if(i < row.size() - 1) out << ",";
			}
			out << "\n";
		}
	}
}
