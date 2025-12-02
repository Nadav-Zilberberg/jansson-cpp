Jansson README
==============

.. |tests|  image:: https://github.com/akheron/jansson/workflows/tests/badge.svg
.. |appveyor|  image::  https://ci.appveyor.com/api/projects/status/lmhkkc4q8cwc65ko

|tests| |appveyor|

Jansson_ is a modern C++ JSON library for encoding, decoding and manipulating JSON
data. This repository has been converted from the original C implementation to
modern C++ with RAII, smart pointers, exceptions, namespaces, classes, and type
traits. Its main features and design principles are:

- Simple and intuitive API and data model

- `Comprehensive documentation`_

- No dependencies on other libraries

- Full Unicode support (UTF-8)

- Extensive test suite using GoogleTest

- Modern C++17 or later implementation

Jansson is licensed under the `MIT license`_; see LICENSE in the
source distribution for details.

Compilation and Installation
----------------------------

This is a CMake-based project. To build and install:

   $ mkdir build && cd build
   $ cmake ..
   $ make
   $ make install

To run the test suite with coverage analysis::

   $ cd tests
   $ mkdir build && cd build
   $ cmake ..
   $ make
   $ ./jansson_tests
   $ lcov --ignore-errors mismatch --directory . --capture --output-file coverage.info
   $ genhtml coverage.info --output-directory coverage_html

The coverage report will be generated in the ``coverage_html`` directory.

Documentation
-------------

Documentation is available at http://jansson.readthedocs.io/en/latest/.

The documentation source is in the ``doc/`` subdirectory. To generate
HTML documentation, invoke::

   $ make html

Then, point your browser to ``doc/_build/html/index.html``. Sphinx_
1.0 or newer is required to generate the documentation.

Community
---------

* `Documentation <http://jansson.readthedocs.io/en/latest/>`_
* `Issue tracker <https://github.com/akheron/jansson/issues>`_
* `Mailing list <http://groups.google.com/group/jansson-users>`_
* `Wiki <https://github.com/akheron/jansson/wiki>`_ contains some development documentation

.. _Jansson: http://www.digip.org/jansson/
.. _`Comprehensive documentation`: http://jansson.readthedocs.io/en/latest/
.. _`MIT license`: http://www.opensource.org/licenses/mit-license.php
.. _Sphinx: http://sphinx.pocoo.org/
