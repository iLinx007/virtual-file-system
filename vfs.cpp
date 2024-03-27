#include "vfs.hpp"



VFS::VFS(){

	root = new Inode("/", nullptr, 1, 0, getTime());            // Initializing root, curr_node and prev_inode in constructor
	curr_inode = root;
	prev_inode = nullptr;

}

// Method defining the destructor
VFS::~VFS(){
	clear_all(root);                                      // Calling the clear_all function that deallocates memory for all nodes
}
 

string VFS::getTime(){
	time_t curr_time = time(0);
    tm *get_time = localtime(&curr_time);                            // Using the ctime header file to get the current time
    
    string my_time = to_string(get_time->tm_mday) + '-' + to_string(get_time->tm_mon+1) + '-' + to_string(get_time->tm_year+1900);     // Writing the time in the appropriate format
    
    return my_time;    //return the formatted time as a string
}


// Method to print instructions that help the user navigate the Virtual FIle System
void VFS::help(){

	cout << "1. help: Prints the list of commands for navigating an using the Virtual File System." << endl;
	cout << "2. pwd: Shows the path of the current working directory." << endl;
	cout << "3. ls: Displays the list of contents in the current working directory." << endl;
	cout << "4. mkdir <folder_name>: Creates a new folder with the given name in the current working directory." << endl;
	cout << "5. touch <file_name> size: Creates a new file with the given name in the current working directory." << endl;
	cout << "6. cd <path>: Change the current working directory to the specified file path." << endl;
	cout << "7. rm <file_name>: Remove a file with the given file name from the current working directory." << endl;
	cout << "8. size <folder name or file name>: Display the size of the given folder or file name." << endl;
	cout << "9. showbin: Show first content of the recycle bin." << endl;
	cout << "10. emptybin: Delete contents of the recycle bin." << endl;
	cout << "11. exit: Exit the virtual file system." << endl;

}

string VFS::pwd(){

	Inode* curr_node = curr_inode;                              // Creating a new node that will be used to iterate over the ancestors of curr_inode
	string pwd;

	while (curr_node->parent != nullptr){                       // While loop to traverse the path of the curr_inode till the root 
		if (pwd.empty()){
			 pwd = pwd + curr_node->name;                       // Printing the name of each ancestor in the right format
			 curr_node = curr_node->parent;
		}
		else{
			pwd = curr_node->name + '/' + pwd;
			curr_node = curr_node->parent;
		}
	}
	pwd = '/' + pwd;                                             // Adding the last back slash to represent the system directory
	
	return pwd;													 // Returning the string of path of the curr_inode
}

void VFS::ls(string command){

	if (command == "sort"){                                                         // Implementing the bublesort for when the sort command is passed
		for (int i = 0; i < curr_inode->children.size()-1; i++){
			for (int j = 0; j < curr_inode->children.size()-1; j++){
				if (curr_inode->children[j+1]->size > curr_inode->children[j]->size){
					swap(curr_inode->children[j], curr_inode->children[j+1] );
				}
			}
		}

	}

	if (curr_inode->type == 0) throw runtime_error("Current working directory is not a folder");       // Handling the case where curr_inode is pointing to a file

	else {
		for (int i = 0; i < curr_inode->children.size(); i++){
			if (curr_inode->children[i]->type == 0){
				cout << "fil " << curr_inode->children[i]->name << " " << curr_inode->children[i]->cr_time << " " << curr_inode->children[i]->size << " bytes" << endl;                  // Listing files and its attributes in the current working directory
			}

			else if (curr_inode->children[i]->type == 1){
				cout << "dir " << curr_inode->children[i]->name << " " << curr_inode->children[i]->cr_time << " " << curr_inode->children[i]->size << " bytes" << endl;					// Listing folders and its attributes in the current working directory
			}		 
		}
	}
}


// Method to create a folder with a given folder name and size
void VFS::mkdir(string folder_name){

	if (folder_name == ""){
		throw runtime_error("Please enter a folder name");                // Handling the case wehre no foldername is typed when creating a new folder
	}

	for (int i = 0; i < curr_inode->children.size(); i++){                // Handling the case where the foldername already exists
		if (curr_inode->children[i]->name == folder_name){
			cout << "Folder name already exists" << endl;

			return;
		} 
	}

	Inode* new_dir = new Inode(folder_name, curr_inode, 1, 10, getTime());     // Creating a new inode with the given foldername

	curr_inode->children.push_back(new_dir);                                   // Adding the new inode(folder) to the current working directory
	
	updateSize(new_dir);													   // Updating the size of the current directory and its ancestors.

}


// Method to create a file with a given file name and size
void VFS::touch(string file_name, unsigned int size){

	if (file_name == ""){
		throw runtime_error("Please enter a file name");                       // Handling the case where no filename is typed when creating a new folder
	}

	for (int i = 0; i < curr_inode->children.size(); i++){                     // Handling the case where the filename already exists
		if (curr_inode->children[i]->name == file_name){
			cout << "File name already exists" << endl;

			return;
		} 
	}

	Inode* new_file = new Inode(file_name, curr_inode, 0, size, getTime());    // Creating a new inode with the given filename and size

	curr_inode->children.push_back(new_file);                                  // Adding the new inode(file) to the current working directory
	updateSize(new_file);                                                      // Updating the size of the current directory and its ancestors.
}


// Method to change current working directory
void VFS::cd(string path){


	if (path == ""){                                             // Checking if the path is an empty string and changing the current node to the root directory
		Inode* temp_inode = curr_inode;                          
		curr_inode = root;                              
		prev_inode = temp_inode;
		return;
	}

	else if (path == ".."){                                      // Checking if the path is .. and changing the curr_inode to the its parent
		if (curr_inode->parent != nullptr){
			Inode* temp_inode = curr_inode;
			curr_inode = curr_inode->parent;
			prev_inode = temp_inode;

			return;
		}
		cout << "Already at root directory. " << endl;
		return;
		
	}

	else if (path == "-"){                                      // Checking if the path is - and changing the curr_inode to the prev_inode
		Inode* temp_inode = curr_inode;

		if (prev_inode != nullptr){
			curr_inode = prev_inode;
			prev_inode = temp_inode;
			return;
		}
		else{
			throw runtime_error("Can not go back");
		}

	}

	else if (path[0] != '/'){                                   // Condition to switch when the path starts from the current directory

		stringstream my_path;
		my_path.str(path);                                      // Writing the path into a astring stream for splitting at /
		string directory; 
		char dummy_slash = '/';
		Inode* temp_inode = curr_inode;

		while(!my_path.eof()){                                  // Reading as long as end of path is not reached
			getline(my_path, directory, dummy_slash);           // Slicing the string path at the /
			if (fol_path(temp_inode, directory) == nullptr || temp_inode->type != 1){                // Handling the case where the string does not exist as a child of the current node
				throw runtime_error("Invalid path");
			}
			else{
				temp_inode = fol_path(temp_inode, directory);               // setting temp_node to the node returned from the getChild method
			}
		}
		if (temp_inode->type != 1){
			throw runtime_error("Invalid path");
		}

		prev_inode = curr_inode;
		curr_inode = temp_inode;                                // Setting the curr_inode to the valid path
		return;
	}

	stringstream my_path;                                       // Condition to switch when the path starts from the root directory
	my_path.str(path);                                          // Writing the path into a astring stream for splitting at /
	char root_dir;
	string directory;
	char dummy_slash = '/';

	my_path >> root_dir;

	if (root_dir != '/'){                                       // Handling the case when there is an invalid path
		throw runtime_error("Invalid path");
		return;
	}
	else{

		Inode* temp_inode = root;                               // Initiating the temp_node to the root

		while(!my_path.eof()){
			getline(my_path, directory, dummy_slash);           // Slicing the string path at the /
			if (fol_path(temp_inode, directory) == nullptr || temp_inode->type != 1){                // Handling the case where the string does not exist as a child of the current node
				throw runtime_error("Invalid path");
			}
			else{
				temp_inode = fol_path(temp_inode, directory);
			}

		}
		prev_inode = curr_inode;
		curr_inode = temp_inode;                                // Setting the curr_inode to the valid path
		return;
	}
}


// Method to get child from the current working directory
Inode* VFS::getChild(string childname){

	for (int i = 0; i < curr_inode->children.size(); i++){         // Looping over the current working directory to find the a child with the given name
		if (curr_inode->children[i]->name == childname){
			return curr_inode->children[i];                        // Returning the node with the given name
		}
	}
	return nullptr;                                                // Returning nullptr if the child is not found
}

Inode* VFS::fol_path(Inode* node, string childname){

	for (int i = 0; i < node->children.size(); i++){         // Looping over the current node parameter to find the a child with the given name
		if (node->children[i]->name == childname){
			return node->children[i];                        // Returning the node with the given name
		}
	}
	return nullptr;                                                // Returning nullptr if the child is not found
}


// Method to remove a file or folder
void VFS::rm(string file_name){
	Inode* item = getChild(file_name);                           // Get the current inode with the specified item
	string item_path = pwd();                                    // Get the path of the curr_inode
	bool found = false;

	if (item != nullptr){                                       // Deleting the inode if it is found
		bin.enqueue(item);
		bin_paths.enqueue(item_path);
		for (int i = 0; i < curr_inode->children.size(); i++){  // Looping through the current directory to find the file or folder with the given name
			if (curr_inode->children[i]->name == file_name){
				curr_inode->children.erase(i);
				found = true;
				break;
			}
		}
	}
	if (found == false){                                          // Returning an error message if the file to delete does not exist.
		cout << file_name << "not in current directory" << endl;
	}
}

// Method to get the node at the given path
Inode* VFS::getNode(string path){

	Inode* saved_prev_inode = prev_inode;                          // Save the current and previous node in new nodes to restore them later
	Inode* saved_curr_inode = curr_inode;

	try {                                                          // Changing the current inode to the node at the given path
		cd(path);
	}
	catch(runtime_error& e){
		cerr << "Invalid path" << endl;                            // Handling errors from the cd method
	}

	Inode* new_node = curr_inode;

	prev_inode = saved_prev_inode;                                 // Restoring the previous and current node to its original state
	curr_inode = saved_curr_inode;

	return new_node;
}


// Method to get the size of the current node
void VFS::size(string path){

	if (path == '/'){
		cout << root->size << " bytes" << endl;
	}

	if (path == ""){
		cout << curr_inode->size << " bytes" << endl;               // Printing the size of the current inode if no path is passed
		return;
	}

	Inode* getSize = getNode(path);                                 // Creating a pointer to the node at the path given
	cout << getSize->size << " bytes" << endl;                      // Printng the size of the node at the given path
}



// Method to update the size of all ancestors of a given node
void VFS::updateSize(Inode *ptr){ 

	Inode* current = ptr; 
	int new_size = current->size;                                          // creating a pointer to traverse the ancestors of the current inode

	while(current->parent != nullptr){                              // using a while loop to traverse the ancestors
		current->parent->size += new_size;                     // Updating the size of the parent node(by adding the size of the current node)
		current = current->parent;                                  // Setting the pointer to the parent node of the current node
	}
}

void VFS::showbin(){
	if (bin.isEmpty()){
		cout << "Bin is empty" << endl;                             // Handling the case where the bin is empty.
		return;
	}
	else{
		Inode* bin_1 = bin.front_element();                         // Getting the first element of the bin.
		string bin_1_path = bin_paths.front_element();              // Getting the first element of the bin_paths.

		cout << bin_1->name << " " << bin_1_path << endl;           // Displaying the name of the current node and its path
	}
}



// Method to empty the bin
void VFS::emptybin(){
	if (bin.isEmpty()){                                            // Handling the case where the bin is empty
		cout << "Bin is empty" << endl;
		return;
	}
	while(!bin.isEmpty()){

		Inode* del_inode = bin.dequeue();                          // Dequeue the first element in the node and storing it in del_node
		bin_paths.dequeue();

		delete del_inode;                                           // Delete the node.
	}
	
	return;
}

void VFS::clear_all(Inode* node){

	if (node->type == 0){                                         // Base case - deleting the current node if its a file
		delete node;
		return;
	}

	for (int i = 0; i < node->children.size(); i++){
		clear_all(node->children[i]);                       
	}
	delete node;
	return;
}

// Method to find and print the paths of all nodes with a given name
void VFS::find(string name){

	Inode* temp = root;
	Inode* or_node = curr_inode;
	find_helper(temp, name);                                   // Calling the recursive helper method to find nodes with the given name
	curr_inode = or_node;                                      // Restoring the current node to its original state


}


// Recursive method to check if a given child is present under specific Inode or not
void VFS::find_helper(Inode* node, string name){

	if (node->type == 0 && node->name == name){                                         // Base case - checking if the current node has the same given name
		curr_inode = node;
		cout << pwd() << endl;                                                          // Printing the path of the matching file name
	}
	else if (node->type == 1 && node->name == name){                                         // Base case - checking if the current node has the same given name
		curr_inode = node;
		cout << pwd() << endl;                                                           // Printing the path of the matching folder name
	}

	for (int i = 0; i < node->children.size(); i++){
		find_helper(node->children[i], name);                                           // Recursively calling the helper method to traverse the list
	}
}



// Exit method
void VFS::exit(){
}