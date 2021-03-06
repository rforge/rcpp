# -*- mode: R; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
#
# zzz.R: install the patches
#
# Copyright (C) 2010	Romain Francois
#
# This file is part of patches.
#
# patches is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# patches is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with patches.  If not, see <http://www.gnu.org/licenses/>.

hijack <- function(name, value, namespace = "base" ){
	namespace <- asNamespace( namespace )
	unlockBinding( name, namespace )
	assign( name, value, envir = namespace )
	lockBinding(name, namespace )
}


.onLoad <- function(libname, pkgname){
	m <- Module( "patches" )
	hijack( "sequence", m$sequence )
}    


