#include "visib_init.hpp"
#include "graph.cpp"

void displayGraphVisib(Graph g, float x1[], float y1[], float t[])
{
	int a=0;
    // Display vertices
    std::cout << "Display vertices" <<  std::endl;
    typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;
    std::pair<vertex_iter, vertex_iter> vertexPair;
    for (vertexPair = vertices(g); vertexPair.first != vertexPair.second; ++vertexPair.first)
    {
        std::cout << g[*vertexPair.first].id << " " << g[*vertexPair.first].label << " " << g[*vertexPair.first].x << " " << g[*vertexPair.first].y << " " << g[*vertexPair.first].orientation << std::endl;

	x1[a]=g[*vertexPair.first].x;
	y1[a]=g[*vertexPair.first].y;
	t[a]=-(g[*vertexPair.first].orientation);
	a+=1;
    }
    
    // Display edges weights
    std::cout << "Display edges weights" <<  std::endl;
    boost::property_map<Graph, boost::edge_weight_t>::type EdgeWeightMap = get(boost::edge_weight_t(), g);
    typedef boost::graph_traits<Graph>::edge_iterator edge_iter;
    std::pair<edge_iter, edge_iter> edgePair;
    for(edgePair = edges(g); edgePair.first != edgePair.second; ++edgePair.first)
    {
        std::cout << boost::source(*edgePair.first, g) << " --" << EdgeWeightMap[*edgePair.first] << "--> "<< boost::target(*edgePair.first, g) << std::endl;
    }
}


using namespace std;


void Ecriture_carte_visib()
{
   	int n1=1000;
	int n2=1000;
	int m=6;
	int x[]={0,0,0,0,0,0};//colonne
    	int y[]={0,0,0,0,0,0};//ligne     
	float t[m-1];//={pi/2,pi/2,0,0,0,0}; 
	
	int i,j,k,l,o,p=0,q=0,r=0,s=0,u=0,w=0,a;
	float dist=1.0 ;
	int pix[n1][n2],pix2[n1][n2];
	int v[m][m];
	float yn[m];
	float x1[5],y1[5];
    	float angle[m-1];//={0.0,0.0,0.0,0.0,0.0,0.0};
	float alphamax[m-1];//={pi/2,pi/2,pi/2,pi/2,pi/2,pi/2};
	int distancemax;
	int distancemin;

	float distancem=2;
	float xn;
	//1px = 0.494134897 cm
	// x px = 337cm

	Graph Graph_test = xmlToGraph("graph.xml");
    	displayGraphVisib(Graph_test,x1,y1,t);

	
	for(a=0;a<m;a++)
	{	
		//printf("*************************** x1 = %lf \t y1 = %lf *********************\n",x1[a],y1[a]);
		x[a]=x1[a]*100/0.494134897;
		y[a]=y1[a]*100/0.494134897;
		alphamax[a]=pi/2;
		//printf("*************************** x1 = %d \t y1 = %d *********************\n",x[a],y[a]);
	}


	distancemax=distancem*100/0.494134897;
	ROS_INFO("Dmax = %d",distancemax);
	distancemin=20/0.494134897;	
	ROS_INFO("Dmin = %d",distancemin);

    	ofstream fichier("src/TurtleBot-pkgs/turtlebot_proj_nav/map/visib.pgm", ios::out | ios::trunc);  
		// ouverture en écriture avec effacement du fichier ouvert

   		if(fichier && (r<1))
        		{	
				ROS_INFO("Creation de la carte");
				fichier << "P2" << endl;
				fichier << "#Thibaut" << endl;
				fichier << "#Carte de visibilite" << endl;
				fichier << n1 << " " << n2 << endl;
				fichier << "15" << endl;
			}
			//else
                		//cerr << "Impossible d'ouvrir le fichier !" << endl;

    
		for (i=0;i<n2;i++) // colonne
		{
			for (j=0;j<n1;j++) // ligne
			{
				pix[j][i]=15;
				//pix2[i][j]=1000000;
				for(k=0;k<m;k++) // amers
				{
					
					/*if(t[k]>-90 && t[k]<90) //si theta compris entre -90 degre et 90 degre
						v[k][1]=-y[k];
					else if (t[k]<-90 && t[k]>90)
						v[k][1]=x[k];
						else v[k][1]=90;*/
				
					//yn[k]=2;//cos(t[k]);
					
					dist = sqrt(pow(i-y[k],2)+pow(j-x[k],2));

					// xn = -1; yn[k] = tan(t[k])*xn;
					if(t[k]<3*pi/2 && t[k]>pi/2)
						xn = 1;
					else 	xn = -1;				
					
					if(t[k]<pi)
						yn[k] = -(tan(t[k])*xn);
					else yn[k] = (tan(t[k])*xn);
					
					
					angle[k]=acos(((j-x[k])*xn+((i-y[k])*yn[k]))/(sqrt(pow((j-x[k]),2)+pow((i-y[k]),2))*(sqrt(pow(xn,2)+pow(yn[k],2)))));					
					//printf("Angle : %f \n", angle[k]);

					if((dist<=distancemax && dist>=distancemin) && (angle[k]<=alphamax[k] && angle[k]>=-alphamax[k]) && (pix[j][i]!=0) )
					{	
						pix[j][i]-=3;
					}
					else 
						if(dist == 0.0)
						{	pix[j][i]=0;
							//printf("Amers :\ni=%d\tj=%d\n",i,j);
							
						}
				}

				//if(i<n&&j<n)
				//printf("%d ", pix[j][i]);
				
				q+=1;
				if(r<1)	//flag pour savoir si l'ecriture s'est deja faite 1 fois
				{	fichier << pix[j][i] << " " ;
					p+=1;
				}
				if(p>=70 && (r<1))// condition de retour a la ligne
				{
					fichier << endl;
					p=0;		
				}
				else if((p<70 && (r==1)))
				{
					ROS_INFO("Integer false, image non generee correctement !");
				}
			}
		}

		//printf("i=%d\tj=%d\n",i,j); //Verification iterations ligne et colonne
		//printf("q=%d\n",q); //Verification nb totale d'iterations

		if(r<1)      		        
        	{	
			fichier.close();
			ROS_INFO("Creation de la carte terminee \n________________________________________________________");
			r=1;
		}
		
}

table pgm_imread(char *argv)			//reads pgm image
	{
	char line[80];
	int imagetype = 0, cols, rows, maxintensity, p2read;
	int **data;
	unsigned char p5read;
	stringstream buffer;
	ifstream infile(argv);			//opens image
	if (infile == NULL)
    		{
	        cerr<<argv[1]<<" either cannot be read or does not exist\n";
        	exit(0);
	        }
	infile.getline(line, 80);
	if(line[0] == 'P')
		switch(line[1])
			{
			case '5' :	imagetype = 1; cout<<"P5 PGM image detected\n"; 		break; //raw image
			case '2' :	imagetype = 2; ROS_INFO("P2 PGM image detected"); 		break; //ASCII image
			default	 :	imagetype = 0; cerr<<"unsupported PGM image format\n"; exit(0); break;
			}
	else
		{
		cerr<<"Invalid image format\n";
		exit(0);
		}
	while (infile.peek()=='#')
        	infile.getline(line,80); 	//read all the comments and oomit them.
	infile >> cols >> rows >> maxintensity;	//read the no of coloumns rows and pixel intensity
	data = new int* [cols];		//memory allocation
	if (!data)
		{
      	     cout << "allocation failure in matrix";
	     exit(1);
    		}
	for(int i = 0; i<cols;i++)
		{
		data[i] = new int[rows];	//making allocation for 2d matrix
	        if (!data[i])
			{
	       		cout << "allocation failure in matrix";
	       		exit(1);
      			}
		}
	if(imagetype == 1)			//raw image mode
		for (int i=0;i<cols;i++)
        	{
        	 for (int j=0;j<rows;j++)
			{
         	        infile>>p5read;
			data[i][j] = (int)p5read;
			//printf("data[%d][%d]= %d \n",i,j,data[i][j]);
			}
	        }
	else if(imagetype == 2)			//ASCII image mode
		{
		buffer << infile.rdbuf();
		for (int i=0;i<cols;i++)
       			{
       			 for (int j=0;j<rows;j++)
				{
       			        buffer>>p2read;
				data[i][j] = p2read;
				//printf("data[%d][%d]= %d \n",i,j,data[i][j]);
				}
       			 }
		}
	table image = {data, cols, rows};
	ROS_INFO("Image read complete");
	infile.close();
    	return image; 		 		//returns the image as a 2-D array in a structure
}
