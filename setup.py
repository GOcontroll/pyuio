from setuptools import setup, Extension

setup(
	name="pyuio",
	version="0.0.1",
	description="A package to expose the Linux Userspace IO to python",
	url="https://github.com/GOcontroll/pyuio",
	author="Maud Spierings",
	author_email="maud_spierings@hotmail.com",
	license="GLP V2.0",
	packages=["pyuio"],
	ext_modules=[Extension("pyuio", ["pyuio.c"])]
	install_requires=[],
	classifiers=[
		"Development Status :: 1 testing",
		"Intended Audience ¨ Embedded Linux Users",
		"License :: GPL V2.0",
		"Operating System :: POSIX :: Linux",
		"Programming Language :: Python :: 3",
	],
)