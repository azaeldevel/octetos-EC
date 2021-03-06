
/*
 * main.cc
 * Copyright (C) 2021 Azael Reyes <azael.devel@gmail.com>
 * 
 * AE is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * AE is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */







#include <fstream>
#include <octetos/core/shell.hh>

#include "trans.hh"


int main(int argc, const char* argv[])
{
	if(argc < 2) 
	{
		std::cerr << "Indique el parametro necesarion\n";
		std::cerr << "trans  dirlog\n";
		return EXIT_SUCCESS;
	}
			
	std::string logDir = argv[1];
	std::string logDirectory;	
	//bool logFile = not logDir.empty();//
	
	//if(logFile) 
	{
		//logDirectory = logDir + "/trans/" + std::to_string(oct::core::getDayID()) + "/" + std::to_string(oct::core::getTimeID()); 
		//logDirectory = logDir + "/trans/" + std::to_string(oct::core::getDayID());
		logDirectory = logDir + "/trans/";
		//oct::core::Shell shell;
		//shell.mkdir(logDirectory,true);
	}
	
	oct::ec::trans::Enviroment* trans = new oct::ec::trans::Enviroment(logDirectory);
	//trans->enableEcho(&std::cout,2);
	
	return trans->run()? EXIT_SUCCESS : EXIT_FAILURE;
}


