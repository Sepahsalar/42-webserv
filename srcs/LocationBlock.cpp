/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnourine <nnourine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 19:33:46 by akovalev          #+#    #+#             */
/*   Updated: 2025/02/14 16:18:35 by nnourine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationBlock.hpp"

LocationBlock::LocationBlock()
{
	_autoindex = false;
	_client_max_body_size = 0;
	_return = std::make_pair(0, "");
}

LocationBlock::LocationBlock(std::string location)
{
	_autoindex = false;
	_client_max_body_size = 0;
	_location = location;
	_return = std::make_pair(0, "");
}

LocationBlock::~LocationBlock()
{
}

std::string LocationBlock::getLocation() const
{
	return _location;
}

std::string LocationBlock::getRoot() const
{
	return _root;
}

std::string LocationBlock::getIndex() const
{
	return _index;
}

bool LocationBlock::getAutoindex() const
{
	return _autoindex;
}

std::vector<std::string> LocationBlock::getCgiExtension() const
{
	return _cgi_extension;
}

std::string LocationBlock::getCgiPath() const
{
	return _cgi_path;
}

std::string LocationBlock::getUploadPath() const
{
	return _upload_path;
}

std::map<int, std::string> LocationBlock::getErrorPages() const
{
	return _error_pages;
}

std::pair<int, std::string> LocationBlock::getReturn() const
{
	return _return;
}

std::string LocationBlock::getAlias() const
{
	return _alias;
}	

size_t LocationBlock::getClientMaxBodySize() const
{
	return _client_max_body_size;
}

std::vector<std::string> LocationBlock::getLimitExcept() const
{
	return _limit_except;
}

void LocationBlock::setClientMaxBodySize(const std::string& client_max_body_size)
{
	if (client_max_body_size.empty() || !std::all_of(client_max_body_size.begin(), client_max_body_size.end(), ::isdigit))
		throw std::invalid_argument("Configuration error: Incorrect client_max_body_size format");
	try {
		int size = std::stoi(client_max_body_size);
		if (size < 0 || size > 1000000000)
			throw std::invalid_argument("Configuration error: client_max_body_size is out of range (0-1000000000)");
		_client_max_body_size = size;
	}
	catch (const std::exception&) {
		throw std::invalid_argument("Configuration error: Incorrect client_max_body_size format");
	}
}


void LocationBlock::setAlias(const std::string& alias)
{
	if (alias.empty())
		throw std::invalid_argument("Configuration error: Alias is empty");
	if (!std::filesystem::exists(alias))
		throw std::invalid_argument("Configuration error: Alias path does not exist");
	_alias = alias;
}

void LocationBlock::setReturn(std::string return_val, const std::string& url)
{
	if (!std::all_of(return_val.begin(), return_val.end(), ::isdigit))
		throw std::invalid_argument("Configuration error: Incorrect return code format");
	int code;
	try {
		code = std::stoi(return_val);
	}
	catch (const std::exception&) {
		throw std::invalid_argument("Configuration error: Incorrect return code format");
	}
	if (url.empty())
		throw std::invalid_argument("Configuration error: Return URL is empty");
	// std::regex url_pattern("^(http|https)://[a-zA-Z0-9.-]+(/.*)?$");
	// if (!std::regex_match(url, url_pattern))
	// 	throw std::invalid_argument("Configuration error: Invalid return URL format");
	if (code < 200 || code > 599) //
		throw std::invalid_argument("Configuration error: Return code is out of range");
	_return = std::make_pair(code, url);
}

void LocationBlock::setLocation(const std::string& location)
{
	_location = location;
}

void LocationBlock::setRoot(const std::string& root)
{
	if (root.empty())
		throw std::invalid_argument("Configuration error: Root is empty");
	if (!std::filesystem::exists(root) || !std::filesystem::is_directory(root))
		throw std::invalid_argument("Configuration error: Root is not a valid directory");
	_root = root;
}

void LocationBlock::setIndex(const std::string& index)
{
	if (index.empty())
		throw std::invalid_argument("Configuration error: Index is empty");
	_index = index;
}

void LocationBlock::setAutoindex(const std::string& autoindex)
{
	if (autoindex == "on")
		_autoindex = true;
	else if (autoindex == "off")
		_autoindex = false;
	else
		throw std::invalid_argument("Configuration error: Incorrect autoindex format");
}

void LocationBlock::setCgiExtension(const std:: vector<std::string>& cgi_extension)
{
	std::set<std::string> valid_extensions = {".sh", ".py", ".cgi", ".rb"};
	if (cgi_extension.empty())
		throw std::invalid_argument("Configuration error: CGI extension is empty");
	for (size_t i = 0; i < cgi_extension.size(); i++)
	{
		if (cgi_extension[i][0] != '.')
			throw std::invalid_argument("Configuration error: Incorrect cgi_extension format");
		else if (valid_extensions.find(cgi_extension[i]) == valid_extensions.end())
			throw std::invalid_argument("Configuration error: Invalid cgi_extension");
		_cgi_extension.push_back(cgi_extension[i]);
	}
}

void LocationBlock::setCgiPath(const std::string& cgi_path)
{
	if (cgi_path.empty())
		throw std::invalid_argument("Configuration error: Cgi path is empty");
	if (!std::filesystem::exists(cgi_path) || !std::filesystem::is_regular_file(cgi_path))
		throw std::invalid_argument("Configuration error: CGI path is not a valid file");
	_cgi_path = cgi_path;
}

void LocationBlock::setUploadPath(const std::string& upload_path)
{
	if (upload_path.empty())
		throw std::invalid_argument("Configuration error: Upload path is empty");
	if (!std::filesystem::exists(upload_path) || !std::filesystem::is_directory(upload_path))
		throw std::invalid_argument("Configuration error: Upload path is not a valid directory");
	_upload_path = upload_path;
}
	
void LocationBlock::setErrorPage(int code, const std::string& page)
{
	if (page.empty())
		throw std::invalid_argument("Configuration error: Error page is empty");
	if (code < 100 || code > 599)
		throw std::invalid_argument("Configuration error: Error code is out of range");
	_error_pages[code] = page;
}

void LocationBlock::setLimitExcept(const std::vector<std::string>& values)
{
	if (values.empty())
		throw std::invalid_argument("Configuration error: Limit except is empty");
	std::set<std::string> valid_methods = {"GET", "POST", "DELETE", "PUT", "HEAD", "OPTIONS"};
	for (size_t i = 0; i < values.size(); i++)
	{
		if (valid_methods.find(values[i]) == valid_methods.end())
			throw std::invalid_argument("Configuration error: Invalid limit except method");
	}
	for (size_t i = 0; i < values.size(); i++)
	{
		_limit_except.push_back(values[i]);
	}
}

void LocationBlock::printLocationBlock()
{
	std::cout << "--------------------------------" << std::endl;
	
	if (_autoindex)
	std::cout << "Autoindex: on" << std::endl;
	else
	std::cout << "Autoindex: off" << std::endl;
	
	if (!_location.empty())
		std::cout << "Location: " << _location << std::endl;
	if (!_root.empty())
	std::cout << "Root: " << getRoot() << std::endl;
	if (!_index.empty())
	std::cout << "Index: " << _index << std::endl;

	if (!_cgi_extension.empty())
	{
		std::cout << "Cgi extension: ";
		for (const std::string& val : _cgi_extension)
		{
			std::cout << val << " ";
		}
		std::cout << std::endl;
	}
	if (!_cgi_path.empty())
	std::cout << "Cgi path: " << _cgi_path << std::endl;
	if (!_upload_path.empty())
	std::cout << "Upload path: " << _upload_path << std::endl;
	if (_return.first)
	std::cout << "Return: " << _return.first << " " << _return.second << std::endl;
	if (!_alias.empty())
	std::cout << "Alias: " << _alias << std::endl;
	if (_client_max_body_size)
	std::cout << "Client max body size: " << _client_max_body_size << std::endl;
	if (!_limit_except.empty())
	{
		std::cout << "Limit except: ";
		for (const std::string& val : _limit_except)
		{
			std::cout << val << " ";
		}
		std::cout << std::endl;
	}
	if (!_error_pages.empty())
	{
		std::cout << "Error pages: " << std::endl;
		for (const std::pair<const int, std::string>& page : _error_pages)
		{
			std::cout << page.first << " " << page.second << std::endl;
		}
	}
}
