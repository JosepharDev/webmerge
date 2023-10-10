#include "../hpp/Client.hpp"


u_int16_t	Client::Cgi::parseCgiWithCrlf(std::string &header, std::string crlf)
{
	enum{statusLine, efieldName, efieldValue};

    size_t sizeLF = crlf.length();
    u_int cursor = efieldName;
    std::string key, value;

    for (size_t i = 0; header[i]; ++i)
    {
        if (cursor == efieldName)
        {
            if (me->_request.isFieldNameValid(header[i]))
            {
                key.push_back(tolower(header[i]));
            }
            else if (header[i] == ':')
                cursor = efieldValue;
            else if (!header.compare(i, sizeLF, crlf) && key.empty())
                return 0;
            else
            {
                return 502;
            }
        }
        else
        {
            if (me->_request.isFieldValueValid(header[i]))
                value.push_back(header[i]);
            else if ( (!header.compare(i, sizeLF, crlf) && key.empty())
                || !header.compare(i, sizeLF*2 , crlf + crlf) )
            {
                if (!key.empty())
                    cgiHeadersMap[key].push_back(value);
                return 0;
            }
            else if (!header.compare(i, sizeLF, crlf))
            {
                cgiHeadersMap[key].push_back(value);
                key = "";
                value = "";
                cursor = efieldName;
				if (sizeLF != 1)
                	i++;
            }
            else
            {
				return 502;
			}
        }
    } 
    return 0;
}


void Client::Cgi::parseCgiHeader()
{
	const int BUFSIZE = 40000;
	char buffer[BUFSIZE];
	std::string sepCgiCrlf;
	me->_response.inputFile.read(buffer, BUFSIZE - 2);

	int	howMuchRead = me->_response.inputFile.gcount();
	cgiHeader.append(buffer, howMuchRead);



	size_t pos = cgiHeader.find("\r\n\r\n");
	size_t posTmp = cgiHeader.find("\n\n");
	if (pos == posTmp && pos == std::string::npos)
	{
		std::cout << "didnot found the end of header or execve failed\n";
		std::string().swap(cgiHeader);
		me->_codeStatus = 500;
		return ;
	}

	sepCgiCrlf = (pos < posTmp) ? "\r\n" : "\n";
	pos = (posTmp < pos) ? posTmp : pos;

	cgibody = cgiHeader.substr(pos + 2);
	cgiHeader.erase(pos + 2);

	if (parseCgiWithCrlf(cgiHeader, sepCgiCrlf))
	{
		me->_response.inputFile.close();
		me->_codeStatus = 502;
		std::cout << "something went wrog with parsing the output ogf cgi\n";
		return ;
	}


    if (cgiHeadersMap.count("status"))
	{
        size_t i = 0;
        u_int8_t j = 0;
        std::string tmpCodeStatus;
        std::string errorDescription;

		std::string &statusHeader = *(cgiHeadersMap["status"].end() - 1);

        while (statusHeader[i] == ' ') ++i;
        while (statusHeader[i] && statusHeader[i] != ' ')
        {
            if (!isdigit(statusHeader[i]) || j > 2)
            {
                me->_codeStatus = 502;
                return ;
            }
            else
                tmpCodeStatus.push_back(statusHeader[i]);
            ++i;
            ++j;
        }
        if (tmpCodeStatus > "599" || j != 3 || statusHeader[i] != ' ')
        {
            me->_codeStatus = 502;
            return ;
        }

        ++i;

        statusLine = std::string("HTTP/1.1 ").append(tmpCodeStatus);
        statusLine.push_back(' ');
        statusLine.append(statusHeader.substr(i));
        statusLine.append("\r\n");
    }
    else
        statusLine = "HTTP/1.1 200 OK\r\n";


		if (!cgiHeadersMap.count("content-type"))
		{
			std::cout << "i should find content type\n";
			me->_codeStatus = 502;
			return ;
		}

		std::map<std::string, std::vector<std::string> >::iterator it;
		std::stringstream ss;
		ss << statusLine;
		std::cout << "--------------------------------------\n";
		for (it = cgiHeadersMap.begin(); it != cgiHeadersMap.end(); ++it)
		{
			std::vector<std::string>::iterator itVector = cgiHeadersMap[it->first].begin();
			for (; itVector != cgiHeadersMap[it->first].end(); itVector++)
			{
				std::cout << "|" << it->first <<"| |" << *itVector << "|\r\n";
				ss << it->first << ": " << *itVector << "\r\n"; 
			}
		}
		std::cout << "--------------------------------------\n";
		
		if (cgiHeadersMap.count("server") == 0)
			ss << "server: servingThings\r\n";

		if (cgiHeadersMap.count("content-length") == 0)
			ss << "transfer-encoding: chunked\r\n";
		else
		{
			std::string &cgiContentLength = *(cgiHeadersMap["content-length"].end() - 1);
			me->_flags.isCgiHaveContentLength = true;
			int i = 0;
			for ( ; cgiContentLength[i] == ' '; ++i);
			for ( ; isdigit(cgiContentLength[i]); ++i);
			for ( ; cgiContentLength[i] == ' '; ++i);
			if (cgiContentLength[i])
			{
				me->_codeStatus = 502;
				return ;
			}
		}

		ss << "\r\n";
		me->_finalAnswer = ss.str();
	}


void		Client::Cgi::sendCgiBodyToFinaleAnswer()
{
	const int BUFERSIZE = 5000;
	char buf[BUFERSIZE + 2];

	// DBG;
	if (cgibody.empty())
	{
		me->_response.inputFile.read(buf, BUFERSIZE);
		int	howMuchRead = me->_response.inputFile.gcount();
			
		if (!me->_flags.isCgiHaveContentLength)
		{
			std::stringstream ss;
			ss << std::hex << howMuchRead;
			me->_finalAnswer = "";
			me->_finalAnswer.append(ss.str().append("\r\n"));

			buf[howMuchRead] = '\r';
			buf[howMuchRead + 1] = '\n';

			me->_finalAnswer.append(buf, howMuchRead + 2);
			
			if (howMuchRead != BUFERSIZE && howMuchRead)
			{
				me->_finalAnswer.append("0\r\n");
				me->_response.inputFile.close();
				me->_flags.isResponseFinished = true; /*make sure this flag is only here*/
			}
		}
		else
		{
			me->_finalAnswer.append(buf, howMuchRead);
			if (howMuchRead != BUFERSIZE)
			{
				me->_response.inputFile.close();
				me->_flags.isResponseFinished = true; 
			}
		}
		if (howMuchRead == 0)
		{
			me->_response.inputFile.close();
			me->_flags.isResponseFinished = true; 
		}

	}
	else
	{
		if (me->_flags.isCgiHaveContentLength == false)
		{
			std::stringstream ss;
			ss << std::hex << cgibody.size();
			me->_finalAnswer = "";
			me->_finalAnswer.append(ss.str().append("\r\n"));

			me->_finalAnswer.append(cgibody.append("\r\n"));
		}
		else
			me->_finalAnswer.append(cgibody);
		cgibody = "";
	}


}

void		Client::Cgi::clearCgi()
{
	cgiHeadersMap.clear();
	cgiHeader.clear();
	cgibody.clear();
	statusLine.clear();
			
	processPid = -1;
	timeStart = 0;

	inputFileCGi.clear();
	outputFileCGi.clear();
			
	cgiKeyProgram.clear();
}


/*even if kill the process you should wait*/
void Client::Cgi::checkCgiTimeout()
{

	if (waitpid(processPid, 0, WNOHANG) == processPid)
	{
		me->_flags.isCgiFinished = true;
		me->_flags.isCgiRunning = false;

		me->_response.inputFile.close();
		me->_response.inputFile.open(me->_cgi.outputFileCGi.c_str(), std::ios::binary);
		if (me->_response.inputFile.is_open() == 0)
		{
			me->_codeStatus = 500;
			std::cout << "open failed to read cgi output\n";
			return ;
		}
	}
	else
	{
		struct timeval tmv;
		gettimeofday(&tmv, NULL);
		size_t timeNow =  tmv.tv_sec;
		if (timeNow - timeStart > 7)
		{
			kill(processPid, SIGKILL);
			waitpid(processPid, 0, WNOHANG);
			me->_codeStatus = 504;
			me->_flags.isCgiRunning = false;
			me->_flags.isCgiFinished = false;
		}
	}
}


void	Client::Cgi::executeCgi()
{
	outputFileCGi = "./../tmp/TTT";
	me->generateRandomName(outputFileCGi);
	std::ofstream outfile (outputFileCGi);
	if (!outfile.is_open())
	{
		std::cout << "failed to open tmpfile output of cgi\n";
		me->_codeStatus = 500;
		return ;
	}
	me->filesToDelete.push_back(outputFileCGi);
	outfile.close();
	processPid = fork();
	if (processPid == 0)
	{
		/*Setup env variables*/
		u_int8_t i = 0;
		size_t envSize = me->_request.requestHeadersMap.count("cookie") ? me->_request.requestHeadersMap.size() : 0;
		envSize += 16;
		char** env = new char*[envSize];

		env[i++] = strdup("SERVER_SOFTWARE=webserver0.0");
		env[i++] = strdup("GATEWAY_INTERFACE=CGI/1.1");
		env[i++] = strdup(std::string("SERVER_NAME=").append(*(me->_request.requestHeadersMap["host"].end()-1)).c_str());
		env[i++] = strdup("SERVER_PROTOCOL=HTTP/1.1");
		env[i++] = strdup("REDIRECT_STATUS=200");
		env[i++] = strdup(std::string("REQUEST_METHOD=").append(me->_request.method).c_str());
		env[i++] = strdup(std::string("PATH_INFO=").append(me->_finalPath).c_str());
		env[i++] = strdup(std::string("QUERY_STRING=").append(me->_request.query).c_str());
		env[i++] = strdup(std::string("SCRIPT_FILENAME=").append(me->_finalPath).c_str());
		if (me->_request.method == "POST" && me->_request.requestHeadersMap.count("content-type"))
		{
			std::string tmp = *(me->_request.requestHeadersMap["content-type"].end() - 1);
			env[i++] = strdup(std::string("CONTENT_TYPE=").append(tmp).c_str());
		}
		else
			env[i++] = strdup(std::string("CONTENT_TYPE=").c_str());

		if (me->_request.requestHeadersMap.count("cookie"))
        {
            std::vector<std::string>::iterator it = me->_request.requestHeadersMap["cookie"].begin();
            while(it != me->_request.requestHeadersMap["cookie"].end())
            {
                env[i++] = strdup(std::string("HTTP_COOKIE=").append(*it).c_str());
                it++;
            }
        }

		if (me->_request.requestHeadersMap.count("content-length"))
		{
			std::string tmp = *(me->_request.requestHeadersMap["content-length"].end() - 1);
			env[i++] = strdup(std::string("CONTENT_LENGTH=").append(me->_finalPath).c_str());
		}
		env[i] = NULL;
		
		me->_response.inputFile.close();
		std::string	&programName 	= me->_configData->allLocations[me->_locationKey].cgi[cgiKeyProgram];
		std::string	&scriptToexec	= me->_finalPath;
		
		if (inputFileCGi.empty() == false) /*for post*/
		{
			if (freopen(inputFileCGi.c_str(), "r", stdin) == NULL)
			{
				std::cerr << "CGI INPUT FAIL\n";
				exit(1);
			}
		}
		if (freopen(outputFileCGi.c_str(), "w", stdout) == NULL)
		{
			std::cerr << "CGI OUTPUT FAIL\n";
			exit(1);
		}

		char *argv[3];
		std::string pathWhereExecute = me->_finalPath.substr(0, me->_finalPath.find_last_of('/'));
		if (chdir(pathWhereExecute.c_str()))
		{
			std::cerr << "chdir FAIL\n";
			exit(1);
		};

		argv[0] = strdup(programName.c_str());
		argv[1] = strdup(scriptToexec.c_str());
		argv[2] = NULL;
		std::cerr << "--" << pathWhereExecute << ";" <<  argv[0] <<  "++" << argv[1] << "\n";
		
		execve(argv[0], argv, env);
		std::cerr << "execve failed\n";
		exit(1);
	}
	else
	{
		struct timeval tmv;
		gettimeofday(&tmv, NULL);
		timeStart =  tmv.tv_sec;

		int ret = waitpid(processPid, 0, WNOHANG);
		if (ret == processPid)
		{
			me->_flags.isCgiFinished = true;
			me->_response.inputFile.close();
			me->_response.inputFile.open(outputFileCGi.c_str(), std::ios::binary);
			if (me->_response.inputFile.is_open() == 0)
			{
				me->_codeStatus = 500;
				std::cout << "open failed to read cgi output\n";
			}
		}
		else if (ret == 0)
			me->_flags.isCgiRunning = true;
		else
			me->_codeStatus = 500;
	}
}