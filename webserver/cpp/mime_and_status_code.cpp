#include "../mime_and_status_code.hpp"


mime_and_status_code::mime_and_status_code()
{
	mime["aac"] = "audio/aac";
	mime["abw"] = "application/x-abiword";
	mime["arc"] = "application/octet-stream";
	mime["avi"] = "video/x-msvideo";
	mime["azw"] = "application/vnd.amazon.ebook";
	mime["bin"] = "application/octet-stream";
	mime["bmp"] = "image/bmp";
	mime["bz"] = "application/x-bzip";
	mime["bz2"] = "application/x-bzip2";
	mime["csh"] = "application/x-csh";
	mime["css"] = "text/css";
	mime["csv"] = "text/csv";
	mime["doc"] = "application/msword";
	mime["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	mime["eot"] = "application/vnd.ms-fontobject";
	mime["epub"] = "application/epub+zip";
	mime["gif"] = "image/gif";
	mime["htm"] = "text/html";
	mime["html"] = "text/html";
	mime["ico"] = "image/x-icon";
	mime["ics"] = "text/calendar";
	mime["jar"] = "application/java-archive";
	mime["jpeg"] = "image/jpeg";
	mime["jpg"] = "image/jpeg";
	mime["js"] = "application/javascript";
	mime["json"] = "application/json";
	mime["mid"] = "audio/midi";
	mime["midi"] = "audio/midi";
	mime["mpeg"] = "video/mpeg";
	mime["mpkg"] = "application/vnd.apple.installer+xml";
	mime["odp"] = "application/vnd.oasis.opendocument.presentation";
	mime["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	mime["odt"] = "application/vnd.oasis.opendocument.text";
	mime["oga"] = "audio/ogg";
	mime["ogv"] = "video/ogg";
	mime["ogx"] = "application/ogg";
	mime["otf"] = "font/otf";
	mime["png"] = "image/png";
	mime["pdf"] = "application/pdf";
	mime["ppt"] = "application/vnd.ms-powerpoint";
	mime["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	mime["rar"] = "application/x-rar-compressed";
	mime["rtf"] = "application/rtf";
	mime["sh"] = "application/x-sh";
	mime["svg"] = "image/svg+xml";
	mime["swf"] = "application/x-shockwave-flash";
	mime["tar"] = "application/x-tar";
	mime["tif"] = "image/tiff";
	mime["tiff"] = "image/tiff";
	mime["ts"] = "application/typescript";
	mime["ttf"] = "font/ttf";
	mime["vsd"] = "application/vnd.visio";
	mime["wav"] = "audio/x-wav";
	mime["weba"] = "audio/webm";
	mime["webm"] = "video/webm";
	mime["webp"] = "image/webp";
	mime["woff"] = "font/woff";
	mime["woff2"] = "font/woff2";
	mime["xhtml"] = "application/xhtml+xml";
	mime["xls"] = "application/vnd.ms-excel";
	mime["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	mime["xml"] = "application/xml";
	mime["xul"] = "application/vnd.mozilla.xul+xml";
	mime["zip"] = "application/zip";
	mime["7z"] = "application/x-7z-compressed";

	status_code[200] = "OK";
	status_code[201] = "Created";
	status_code[400] = "Bad Request";
	status_code[403] = "Forbidden";
	status_code[404] = "Not Found";
	status_code[405] = "Method Not Allowed";
	status_code[408] = "Request Timeout";
	status_code[411] = "Length Required";
	status_code[413] = "Payload Too Large";
	status_code[414] = "URI Too Long";
	status_code[500] = "Internal Server Error";
	status_code[501] = "Not Implemented";
	status_code[505] = "HTTP Version Not Supported";

	mime_reverse ["audio/aac"] = ".aac";
	mime_reverse ["application/x-abiword"]= ".abw";
	mime_reverse ["application/octet-stream"]= ".arc" ;
	mime_reverse ["video/x-msvideo"]= ".avi" ;
	mime_reverse ["application/vnd.amazon.ebook"]= ".azw";
	mime_reverse ["application/octet-stream"]= ".bin";
	mime_reverse ["image/bmp"]= ".bmp";
	mime_reverse ["application/x-bzip"]= ".bz" ;
	mime_reverse ["application/x-bzip2"]= ".bz2";
	mime_reverse ["application/x-csh"]= ".csh" ;
	mime_reverse ["text/css"]= ".css";
	mime_reverse ["text/csv"]= ".csv";
	mime_reverse ["application/msword"]= ".doc";
	mime_reverse ["application/vnd.openxmlformats-officedocument.wordprocessingml.document"]= ".docx";
	mime_reverse ["application/vnd.ms-fontobject"]= ".eot";
	mime_reverse ["application/epub+zip"]= ".epub";
	mime_reverse ["image/gif"]= ".gif";
	mime_reverse ["text/html"]= ".htm";
	mime_reverse ["text/html"]= ".html";
	mime_reverse ["image/x-icon"]= ".ico";
	mime_reverse ["text/calendar"]= ".ics";
	mime_reverse ["application/java-archive"]= ".jar";
	mime_reverse ["image/jpeg"]= ".jpeg";
	mime_reverse ["image/jpeg"]= ".jpg";
	mime_reverse ["application/javascript"]= ".js";
	mime_reverse ["application/json"]= ".json";
	mime_reverse ["audio/midi"]= ".mid";
	mime_reverse ["audio/midi"]= ".midi";
	mime_reverse ["video/mpeg"]= ".mpeg";
	mime_reverse ["application/vnd.apple.installer+xml"]= ".mpkg";
	mime_reverse [ "application/vnd.oasis.opendocument.presentation"]= ".odp";
	mime_reverse ["application/vnd.oasis.opendocument.spreadsheet"]= ".ods";
	mime_reverse ["application/vnd.oasis.opendocument.text"]= ".odt";
	mime_reverse ["audio/ogg"]= ".oga";
	mime_reverse ["video/ogg"]= ".ogv";
	mime_reverse ["application/ogg"]= ".ogx";
	mime_reverse ["font/otf"]= ".otf";
	mime_reverse ["image/png"]= ".png";
	mime_reverse ["application/pdf"]= ".pdf";
	mime_reverse ["application/vnd.ms-powerpoint"]= ".ppt";
	mime_reverse ["application/vnd.openxmlformats-officedocument.presentationml.presentation"]= ".pptx";
	mime_reverse ["application/x-rar-compressed"]= ".rar";
	mime_reverse ["application/rtf"]= ".rtf";
	mime_reverse [ "application/x-sh"]= ".sh";
	mime_reverse ["image/svg+xml"]= ".svg";
	mime_reverse ["application/x-shockwave-flash"]= ".swf";
	mime_reverse ["application/x-tar"]= ".tar";
	mime_reverse ["image/tiff"]= ".tif";
	mime_reverse ["image/tiff"]= ".tiff";
	mime_reverse ["application/typescript"]= ".ts";
	mime_reverse ["font/ttf"]= ".ttf";
	mime_reverse ["application/vnd.visio"]= ".vsd";
	mime_reverse ["audio/x-wav" ]= ".wav";                 
	mime_reverse ["audio/webm"]= ".weba";  
	mime_reverse ["video/webm"]= ".webm";  
	mime_reverse ["image/webp"]= ".webp";      
	mime_reverse ["font/woff" ]= ".woff";  
	mime_reverse ["font/woff2"]= ".woff2";                        
	mime_reverse ["application/xhtml+xml"]= ".xhtml";
	mime_reverse ["application/vnd.ms-excel"]= ".xls";    
	mime_reverse ["application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"]= ".xlsx";
	mime_reverse ["application/xml"] = ".xml";
	mime_reverse ["application/vnd.mozilla.xul+xml"]= ".xul";
	mime_reverse ["application/zip"] = ".zip";
	mime_reverse ["application/x-7z-compressed"]= ".7z";
}