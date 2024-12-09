/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnourine <nnourine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:39:07 by asohrabi          #+#    #+#             */
/*   Updated: 2024/12/09 19:20:04 by nnourine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHANDLER_HPP
#define HTTPHANDLER_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "SystemCallError.hpp"
#include "CGIHandler.hpp"
#include "ServerBlock.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>

class HttpHandler
{
	private:
		CGIHandler					_cgiHandler;
		std::string					_rootDir;
		ServerBlock					&_serverBlock;
		// last one might be better to be const, then
		// getlocations function in serverblock needs to change too
		std::map<int, std::string>	_errorPages;
		size_t						_maxBodySize;
		std::vector<std::string>	_responseParts;

		bool						_isMethodAllowed(const std::string &method, const std::string &path);
		std::string					_getErrorPage(int statusCode);
		std::string					_validateRequest(const Request &req);

	public:
		HttpHandler(ServerBlock &serverConfig); //if serverblock became const, here too
		~HttpHandler();

		std::vector<std::string>	createResponse(const std::string &request);
		std::vector<std::string>	handleRequest(const Request &req);
		std::vector<std::string>	handleGET(const Request &req);
		std::vector<std::string>	handleFileRequest(const std::string &filePath);
		std::vector<std::string>	handlePOST(const Request &req);
		std::vector<std::string>	handleDELETE(const Request &req);
		std::vector<std::string>	handleCGI(const Request &req);
		size_t						getChunkSize(const Request &req);
		std::vector<std::string>	createResponseParts(const Request &req, Response &res);

		std::string					extractFilename(const std::string &disposition);
		void						saveFile(const std::string &filename, const std::string &fileData);
};

#endif
