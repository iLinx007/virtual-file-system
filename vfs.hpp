
#ifndef VFS_H
#define VFS_H
#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<string>
#include<ctime>
#include<fstream>
#include<exception>
#include "inode.hpp"
#include "queue.hpp"
using namespace std;

class VFS
{
	private:
		Inode *root;				//root of the VFS
		Inode *curr_inode;			//current iNode
		Inode *prev_inode;			//previous iNode
		Queue<Inode*> bin;			//bin containing the deleted Inodes
		Queue<string> bin_paths;	//paths of the items in bin
	
	public:	 	
		//Required methods
		VFS();
		~VFS();	
		void help();						
		string pwd();
		void ls(string command);						
		void mkdir(string folder_name);
		void touch(string file_name, unsigned int size);
		void cd(string path);
		void rm(string file_name);
		void size(string path);
		void showbin();
		void emptybin();
		void find(string name);
		void exit();

		//Helper methods
		string getTime();								//return system time as an string
		Inode* getChild(string childname);	    //returns a specific child of given Inode
		Inode* getNode(string path);			//Helper method to get a pointer to iNode at given path
		void updateSize(Inode *ptr);					//re-calculate the size of an Inode and its parents
		void clear_all(Inode* node);                    // Helper method to deallocate memory in destructor
		void find_helper(Inode *ptr, string name);     // Finding the child at any given node
		Inode* fol_path(Inode* node, string childname);   //Helper method to switch path

		
		/*
		//Optional methods
		void mv(string file, string folder);
		void recover();

		//Optional helper methods
		void load(ifstream &fin);				//Helper method to load the vfs.dat
		bool find_helper(Inode *ptr, string name);		//recursive method to check if a given child is present under specific Inode or not
		void write(ofstream &fout, Inode *ptr);			//traverse and write recrusively the vfs data

		*/
};
//===========================================================
#endif
