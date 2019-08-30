//Program: Implementation of  Hex board
//Author: Parul Agrawal
//Date: 9/8/19
//
//Important:
//	1.For storing Hex Board adjcency matrix is used.
//	2.Execution of program needs c++11 support.It is also mandetory for course lectures.
//	3.Read about hex game prior executing program.
//	5.Some changes are made according to implementation.
//  6.Excessive commenting is intentional.
//  7.Do not use comma(,) in between of coordinates,otherwise program will go in for infinite loop
//  8.Implementation is based on monto carlo simulation.
//  9.Wait for a while untill computer chosses its move.




#include<iostream>
#include<vector>
#include<string>
#include<ctime>
#include<utility>
#include<queue>

using namespace std;
class hexBoard                                                                                                       //contains all members and funtions related to hex board
{
	public:
					            hexBoard (int,                                                                
											int); 
					            hexBoard (vector<vector<char> > boardIn,                               
																	int colorIn,
																			int nIn)
								{
									board=boardIn;
									color=colorIn;
									n=nIn;
								}                                                                                
		void	    			printBoard();                                                                         //For printing hex board
        inline bool 	    	isValidMove(int,                                                                      //Check if any move is valid move or not
												int);
		void 	    			move(int,                                                                             //given coordintes, occupies the place in board for player
										int,
											char);
 		bool     				win(int,                                                                              //check if there is any winner
		 								int);
		inline bool 			validPlace(int,                                                                       //check if position lies inside board
											int);
		pair<int,int>           montoCarloMove();
            
 		
	private:
		double  	            probability(int,                                                                      //calculates the probabality of wining on a perticular move.Used in montoCarloMove()
												int);
	    vector<vector<char> >   fill_board();                                                                         //fills whole board randomly.Used in montoCarloMove()
	    bool 					filled(vector<vector<char> >);                                                        //checks whether board is filled fully or not.Used in montoCarloMove()
	    void 	    			checkBorders(int x,                                                                   //finding a border move.Used in win()
												int y, 
													std::vector<bool> &flags, 
																			char side);
		vector<vector<char> >	board;
		int 					n;
		int 					color;
		const static char 		Blue='B';
		const static char 		Red='R';
		const int 				directions[6][2]={{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0}};                            //stores 6 possible neighbour of a node
};

hexBoard::hexBoard(int n,int color):n(n),color(color)																			  
{
	board.resize(n);                                                                                                   //making board size as specified by user
	for(int i = 0; i < n; ++ i)
	{
		board[i].resize(n);                                                                      
	}
	for(int i=0; i < n; ++i)
	{
		for(int j=0; j < n; ++j)
		{
			board[i][j]='.';                                                                                           //initially assingning whole board as blank(.)
		} 
	}
} 


void hexBoard::printBoard()
{
	for(int i=0; i < n; ++i) 
    {
		for(int j=0;j<i;j++)
		{	
			cout<<" ";
		}
		for(int j=0; j < n; ++j) 
        {
            cout<<board[i][j]<<" ";
        }       
		cout<<endl;
	}
}


//if position at x and y coordinates is blank then function return true else false
bool hexBoard::isValidMove(int x,
								int y)
{
	if(board[x][y]=='.')																					
	{
		return true;
	}
	else
	{
		return false;
	}
}

//fill the character corresponding to player in specified coordinates
void hexBoard::move(int x,
						int y,char c)
{
	if(c=='u')
	{
		if(color==0)
		{
			board[x][y]=Blue;	
		}
		else
		{
			board[x][y]=Red;
		}		
	}
	else
	{
			if(color==0)
		{
			board[x][y]=Red;	
		}
		else
		{
			board[x][y]=Blue;
		}	
	}
	//cout<<"move done"<<endl;		
}


bool hexBoard::validPlace(int x,
								int y)
{
	return x>=0 && x<n && y>=0 && y<n;                                                           //checks whether coordinated lies with in the board or not
}
void hexBoard::checkBorders(int x, 
								int y, 
									std::vector<bool> &flags,
															char side) 
{
	if (side == Red)                                                                            //if present player is red it will check for left to right boundry.Hence it is checking for y coordinate
	{
		if (y == 0)																	            //because y will be either 0 or n at left to right boundries
			flags[0] = true;
		if (y == n - 1)
			flags[1] = true;
	}
	else if (side == Blue)															           //if present player is blue it will check for top to bottom boundry.Hence it is checking for x coordinate
	{																				           //because x will be either 0 or n at top to bottom boundries
		if (x == 0)
			flags[0] = true;
		if (x == n - 1)
			flags[1] = true;
	}
}


//for winner finding we have used BFS i.e using a queue and travesing its child
bool hexBoard::win(int x,
						int y)
{
	if (validPlace(x, y) == false || board[x][y] == '.')                                       //if position is not valid then function returns
	{
		return false;	
	}
	
	
	char 					side = board[x][y];                                               //finds the color of present move
	vector<bool> 			flags(2, false);                                                  //if any boundry is  touched by its specified color then flag gets checked
	vector<vector<bool> > 	visited(n, vector<bool>(n));                                      //keeping track og visited nodes
	queue<pair<int, int> > 	traces;                                                           //stores the path
	
	traces.push(make_pair(x, y));
	visited[x][y] = true;

	while (!traces.empty())
	{
		auto top = traces.front();                                                           //item is poped out 
		checkBorders(top.first, top.second, flags, side);                                    //if it is edge move then it marks flags 1
		traces.pop();                          
		for (int n=0; n<6; n++)														        //visiting every of neighbour and if they are valid neighbours then pushing them to queue
		{ 
			int X = top.first + directions[n][0];
			int Y = top.second + directions[n][1];
			if (validPlace(X, Y) && 
								board[X][Y] == side && 
									visited[X][Y] == false)
			{
				visited[X][Y] = true;
				traces.push(make_pair(X, Y));
			}
		}
	}
	return flags[0] && flags[1];	                                                         //when both edges are visited then function will return true
}


bool hexBoard::filled(vector<vector<char> >board)
{
	for(int i=0; i < n; ++i)
	{
		for(int j=0; j < n; ++j)
		{
			if(board[i][j] == '.')                                                           //checking if position is blank
			{
			return false;                                                                    
			} 
		}
		
	}
	return true;
}


//filling board randomly by machine and user moves
vector<vector<char> > hexBoard::fill_board()
{
	int x,y;
	int turn=0;
	do
	{
		turn=!turn;
		do
		{
			x=rand()%n;
			y=rand()%n;
		}while(!isValidMove(x,y));
		if(turn)
		{
			move(x,y,'u');
		}
		else
		{
			move(x,y,'m');
		}	
	}while(!filled(board));
	return board;	
}




//function find the move with highest probability of winning
double hexBoard::probability(int x,int y)
{
	int winTimes=0;
	bool winner=false;
	for(int i=0;i<100;i++)
	{
		hexBoard filledBoard(board,color,n);                                             //new instance of the board                                           
		filledBoard.move(x,y,'m');                                                       //placing the current move
		vector<vector<char> > fBoard=filledBoard.fill_board();                           //filling the board randomly by machine and ser move
		if(color==0)
		{
			for(int j=0;j<n;j++)
			{
				if(fBoard[0][j]==Red)
				{
					winner=filledBoard.win(0,j);                                       //finding if machine is winner
					if(winner)
					{
						winTimes++;
						break;
					}
				}	
			}	
		}
		else
		{
			for(int j=0;j<n;j++)
			{
				if(fBoard[j][0]==Blue)
				{
					winner=filledBoard.win(j,0);                                      //finding if machine is winner
					if(winner)
					{
						winTimes++;
						break;
					}
				}	
			}
		}
	}
	return (static_cast<double>(winTimes)/100);                                       //returning the probablity of winning out of 100 trials
	
}

pair<int,int> hexBoard::montoCarloMove()
{
		double maxProb=0;
		pair<int,int> move;
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				if(isValidMove(i,j)&&validPlace(i,j))
				{
					double x=probability(i,j);
					if(x>maxProb)                                                       //finding maximum probability move out of remaning moves
					{
						maxProb=x;
						move=make_pair(i,j);	
					}
				
					
				}
			}
		}
		return move;
}


//Execution of program starts here
int main()
{
	srand(time(0));
	pair<int,int> moved;                        																 //seed for random no. generator  
	cout<<"Enter the dimension for board."<<endl;
	int n;
	cin>>n;
	cout<<"For choosing Blue color press 0, for Red press 1"<<endl;
	int color;
	cin>>color;
	if(color==0)                                                                                                //specifying color according to color chosen by user
	{
		cout<<"Your boundry is Top to bottom"<<endl;
	}
	else
	{
		cout<<"Your boundry is Left to right"<<endl;
	}
	hexBoard h(n,color);                                                                                        //creating hex board
	h.printBoard();
	int turn=0;																									//storing turn of user and computer
	int steps=0;																								//stores total steps used befor winning game.It counts user and computer steps individually
	int x,y;
	while(1)  				
	{
		steps++;
		turn=!turn;                                                                                             //reversing the turn
		if(turn)
		{
			do                                                                                                  //executes untill a valid move comes
			{
				cout<<endl<<"Enter the coordinates of your move seprated by space."<<endl; 
				cin>>x>>y;																						//taking user input for move
			}while(!(h.isValidMove(x,y)&&h.validPlace(x,y)));
			h.move(x,y,'u');
			cout<<endl;
			h.printBoard();
				
		}
		else
		{
			cout<<endl<<"Computer move:"<<endl;
			moved=h.montoCarloMove();																		   //computer will decide its move by monto carlo simulation
			cout<<moved.first<<" "<<moved.second<<endl;
			h.move(moved.first,moved.second,'m'); 
			x=moved.first;
			y=moved.second;                                                                          
			cout<<endl;
			h.printBoard();
        }
        if (h.win(x, y))																						//check if anybody wins
		{
			cout<<"Congratulations!! ";
			cout << (turn ? "user" : "computer") << " win " << endl<<endl;;                                     //prints winner
			cout << "Total steps = " << steps << endl<<endl<<endl;                                                    //prints no. of steps used in winning
			h.printBoard();
			cout<<endl<<endl;
			break;
		}
		
	}	
	return 0;
}
