
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

#if defined(__GNUC__) && defined(__linux__)
	#include <octetos/core/shell.hh>
#elif defined(__GNUC__) && (defined(_WIN32) || defined(_WIN64))
    #include <shell.hh>
#else
    #error "Pltaforma desconocida"
#endif

#include "schedule.hh"


int main(int argc, const char* argv[])
{
	oct::core::Time time1;
	time1.read("Vie 23:15","%a %H:%M",std::locale("es_MX.utf-8"));
	time1.print(std::cout,"%c");
	
	std::cout << "\n";
	
	oct::core::Time time2;
	time2.print(std::cout,"%c");
	
	std::cout << "\n";
	
	std::locale mx1("es_MX.utf-8");
	std::locale mx2("es_MX.utf-8");
	
	std::locale en("en_US.utf-8");
	std::locale actual;
	std::cout << "Locale si : " << actual.name() << "\n";
	
	std::cout << "\n";



	return EXIT_SUCCESS ;
}


