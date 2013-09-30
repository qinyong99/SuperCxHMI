if (!window.CxRunCtrl)
{
    window.CxRunCtrl = { };
}

// CxRunCtrl control instance counter for memory mgt
CxRunCtrl._CxRunCtrlCount = 0;
CxRunCtrl.onCxRunCtrlInstalled = function () {window.location.reload(false);};

//////////////////////////////////////////////////////////////////
// isInstalled, checks to see if the correct version is installed
//////////////////////////////////////////////////////////////////

CxRunCtrl.isInstalled = function(version)
{
    if (version == null)
	version = "1.6";

    var uaString = navigator.userAgent;
    var reqVersionArray = version.split(".");
    reqMajorVer = (reqVersionArray[0] != null) ? reqVersionArray[0] : 1;
    reqMinorVer = (reqVersionArray[1] != null) ? reqVersionArray[1] : 6;
    reqBuildVer = (reqVersionArray[2] != null) ? reqVersionArray[2] : 0;
    
    function detectAgControlVersion()
    {
        agVersion = -1;       

        if ((navigator.userAgent.indexOf('Windows') != -1) && (navigator.appVersion.indexOf('MSIE') != -1) )
        {
            try
            {
                var AgControl = new ActiveXObject("CXRUNCTRL.RunCtrl.1");
                agVersion = AgControl.CurrentVersion;                
                AgControl = null;
            }
            catch (e)
            {
                agVersion = -1;
            }
        }
        return agVersion;
    }
    var versionStr = detectAgControlVersion();

    if (versionStr == -1 )
    {
        return false;
    }
    else if (versionStr != 0)
    {
        versionArray = versionStr.split(".");

        var versionMajor = versionArray[0];
        var versionMinor = versionArray[1];
        var versionBuild = (versionArray[2] != null) ? versionArray[2] : 0;

        if (versionMajor > parseFloat(reqMajorVer))
        {
            return true;
        }
        else if (versionMajor == parseFloat(reqMajorVer))
        {
            if (versionMinor > parseFloat(reqMinorVer))
            {
                return true;
            }
            else if (versionMinor == parseFloat(reqMinorVer))
            {
                if (versionBuild >= parseFloat(reqBuildVer))
                {
                    return true;
                }
            }
        }
        return false;
    }
}

CxRunCtrl.WaitForInstallCompletion = function()
{
    if (!CxRunCtrl.isBrowserRestartRequired && CxRunCtrl.onCxRunCtrlInstalled)
    {
//      try
//      {
//          navigator.plugins.refresh();
//      }
//      catch(e)
//      {
//      }
	
	if (CxRunCtrl.isInstalled(null))
        {
            if (CxRunCtrl.HasDetected)
            {
                CxRunCtrl.onCxRunCtrlInstalled();
                return;
            }

            CxRunCtrl.HasDetected = true;
	    setTimeout(CxRunCtrl.WaitForInstallCompletion, 2000);
        }
        else
        {
            setTimeout(CxRunCtrl.WaitForInstallCompletion, 1000);
        }
    }
}
CxRunCtrl.__startup = function()
{
    CxRunCtrl.isBrowserRestartRequired = CxRunCtrl.isInstalled(null);//(!window.ActiveXObject || CxRunCtrl.isInstalled(null));
    if (!CxRunCtrl.isBrowserRestartRequired)
    {
        CxRunCtrl.WaitForInstallCompletion();
    }
    if (window.removeEventListener) { 
       window.removeEventListener('load', CxRunCtrl.__startup , false);
    }
    else { 
        window.detachEvent('onload', CxRunCtrl.__startup );
    }
}

if (window.addEventListener) 
{
    window.addEventListener('load', CxRunCtrl.__startup , false);
}
else 
{
    window.attachEvent('onload', CxRunCtrl.__startup );
}

///////////////////////////////////////////////////////////////////////////////
// createObject();  Params:
// parentElement of type Element, the parent element of the CxRunCtrl Control
// id of type string
// properties of type String, object literal notation { name:value, name:value, name:value},
//     current properties are: width, height, Server, Port, DefaultUser, DefaultPassword, ZoomFitWindow:  all are of type string
// events of type String, object literal notation { name:value, name:value, name:value},
//     current events are onLoad onError, both are type string
// userContext of type Object
/////////////////////////////////////////////////////////////////////////////////

CxRunCtrl.createObject = function(parentElement, id, properties, events, userContext)
{
    var slPluginHelper = new Object();
    var slProperties = properties;
    var slEvents = events;
    
    slPluginHelper.version = slProperties.version;
//  slProperties.source = source;    
    slPluginHelper.alt = slProperties.alt;
    slPluginHelper.width = slProperties.width;
    slPluginHelper.height = slProperties.height;
    
    //rename properties to their tag property names
//  if ( slProperties.isWindowless && !slProperties.windowless)
//      slProperties.windowless = slProperties.isWindowless;
//  if ( slProperties.framerate && !slProperties.maxFramerate)
//      slProperties.maxFramerate = slProperties.framerate;
    if ( id && !slProperties.id)
        slProperties.id = id;
    
    // remove elements which are not to be added to the instantiation tag
//  delete slProperties.ignoreBrowserVer;
//  delete slProperties.inplaceInstallPrompt;
//  delete slProperties.version;
//  delete slProperties.isWindowless;
//  delete slProperties.framerate;
//  delete slProperties.data;
//  delete slProperties.src;
    delete slProperties.alt;


    // detect that the correct version of CxRunCtrl is installed, else display install

    if (CxRunCtrl.isInstalled(slPluginHelper.version))
    {
        //move unknown events to the slProperties array
        for (var name in slEvents)
        {
            if ( slEvents[name])
            {
                if ( name == "onLoad" && typeof slEvents[name] == "function" && slEvents[name].length != 1 )
                {
                    var onLoadHandler = slEvents[name];
                    slEvents[name]=function (sender){ return onLoadHandler(document.getElementById(id), userContext, sender)};
                }
                var handlerName = CxRunCtrl.__getHandlerName(slEvents[name]);
                if ( handlerName != null )
                {
                    slProperties[name] = handlerName;
                    slEvents[name] = null;
                }
                else
                {
                    throw "typeof events."+name+" must be 'function' or 'string'";
                }
            }
        }
        slPluginHTML = CxRunCtrl.buildHTML(slProperties);
    }
    //The control could not be instantiated. Show the installation prompt
    else 
    {
        slPluginHTML = CxRunCtrl.buildPromptHTML(slPluginHelper);
    }

    // insert or return the HTML
    if(parentElement)
    {
        parentElement.innerHTML = slPluginHTML;
    }
    else
    {
        return slPluginHTML;
    }

}

///////////////////////////////////////////////////////////////////////////////
//
//  create HTML that instantiates the control
//
///////////////////////////////////////////////////////////////////////////////
CxRunCtrl.buildHTML = function( slProperties)
{
    var htmlBuilder = [];

    htmlBuilder.push('<object CLASSID="CLSID:C162A5CC-6BB1-4C0A-9081-C520A4EADDA7"');
    if ( slProperties.id != null )
    {
        htmlBuilder.push(' id="' + slProperties.id + '"');
    }
    if ( slProperties.width != null )
    {
        htmlBuilder.push(' width="' + slProperties.width+ '"');
    }
    if ( slProperties.height != null )
    {
        htmlBuilder.push(' height="' + slProperties.height + '"');
    }
    htmlBuilder.push(' >');
    
    delete slProperties.id;
    delete slProperties.width;
    delete slProperties.height;
    
    for (var name in slProperties)
    {
        if (slProperties[name])
        {
            htmlBuilder.push('<param name="'+CxRunCtrl.HtmlAttributeEncode(name)+'" value="'+CxRunCtrl.HtmlAttributeEncode(slProperties[name])+'" />');
        }
    }
    htmlBuilder.push('<\/object>');
    return htmlBuilder.join('');
}




// createObjectEx, takes a single parameter of all createObject parameters enclosed in {}
CxRunCtrl.createObjectEx = function(params)
{
    var parameters = params;
    var html = CxRunCtrl.createObject(parameters.parentElement, parameters.id, parameters.properties, parameters.events, parameters.context);
    if (parameters.parentElement == null)
    {
        return html;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Builds the HTML to prompt the user to download and install CxRunCtrl
///////////////////////////////////////////////////////////////////////////////////////////////
CxRunCtrl.buildPromptHTML = function(slPluginHelper)
{
    var slPluginHTML = "";
    var shortVer = slPluginHelper.version ;

    //direct download pointer
    var directDownload = "CxRunCtrl.exe";
   
    if ( slPluginHelper.alt )
    {
        slPluginHTML = slPluginHelper.alt;
    }
    else
    {
        if (! shortVer )
        {
            shortVer="";
        }

        slPluginHTML += "<div style='width: {1}px; height: {2}px; background-color: #FFFFA0; text-align: left; color: #FF0000; font-size: 11px'><a href='javascript:CxRunCtrl.getCxRunCtrl(\"{3}\");'>Web运行包没有安装，请点击此处下载</a></div>";                
        slPluginHTML = slPluginHTML.replace('{1}', slPluginHelper.width);
        slPluginHTML = slPluginHTML.replace('{2}', slPluginHelper.height);
        slPluginHTML = slPluginHTML.replace('{3}', shortVer );
    }
    
    return slPluginHTML;
}

CxRunCtrl.getCxRunCtrl = function(version)
{
    top.location = "cxrunctrl.exe";
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// Encodes special characters in input strings as charcodes
///////////////////////////////////////////////////////////////////////////////////////////////
CxRunCtrl.HtmlAttributeEncode = function( strInput )
{
    var c;
    var retVal = '';

    if (strInput == null)
    {
        return null;
    }
      
    for(var cnt = 0; cnt < strInput.length; cnt++)
    {
        c = strInput.charCodeAt(cnt);

        if (( ( c > 96 ) && ( c < 123 ) ) ||
            ( ( c > 64 ) && ( c < 91 ) ) ||
            ( ( c > 43 ) && ( c < 58 ) && (c!=47)) ||
            ( c == 95 ))
        {
            retVal = retVal + String.fromCharCode(c);
        }
        else
        {
          retVal = retVal + '&#' + c + ';';
        }
    }
      
    return retVal;
}
///////////////////////////////////////////////////////////////////////////////
//
//  Default error handling function to be used when a custom error handler is
//  not present
//
///////////////////////////////////////////////////////////////////////////////

CxRunCtrl.default_error_handler = function (sender, args)
{
    var iErrorCode;
    var errorType = args.ErrorType;

    iErrorCode = args.ErrorCode;

    var errMsg = "\nCxRunCtrl error message     \n" ;

    errMsg += "ErrorCode: "+ iErrorCode + "\n";


    errMsg += "ErrorType: " + errorType + "       \n";
    errMsg += "Message: " + args.ErrorMessage + "     \n";

    if (errorType == "ParserError")
    {
        errMsg += "XamlFile: " + args.xamlFile + "     \n";
        errMsg += "Line: " + args.lineNumber + "     \n";
        errMsg += "Position: " + args.charPosition + "     \n";
    }
    else if (errorType == "RuntimeError")
    {
        if (args.lineNumber != 0)
        {
            errMsg += "Line: " + args.lineNumber + "     \n";
            errMsg += "Position: " +  args.charPosition + "     \n";
        }
        errMsg += "MethodName: " + args.methodName + "     \n";
    }
    alert (errMsg);
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// Releases event handler resources when the page is unloaded
///////////////////////////////////////////////////////////////////////////////////////////////
CxRunCtrl.__cleanup = function ()
{
    for (var i = CxRunCtrl._CxRunCtrlCount - 1; i >= 0; i--) {
        window['__slEvent' + i] = null;
    }
    CxRunCtrl._CxRunCtrlCount = 0;
    if (window.removeEventListener) { 
       window.removeEventListener('unload', CxRunCtrl.__cleanup , false);
    }
    else { 
        window.detachEvent('onunload', CxRunCtrl.__cleanup );
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////
/// Releases event handler resources when the page is unloaded
///////////////////////////////////////////////////////////////////////////////////////////////
CxRunCtrl.__getHandlerName = function (handler)
{
    var handlerName = "";
    if ( typeof handler == "string")
    {
        handlerName = handler;
    }
    else if ( typeof handler == "function" )
    {
        if (CxRunCtrl._CxRunCtrlCount == 0)
        {
            if (window.addEventListener) 
            {
                window.addEventListener('onunload', CxRunCtrl.__cleanup , false);
            }
            else 
            {
                window.attachEvent('onunload', CxRunCtrl.__cleanup );
            }
        }
        var count = CxRunCtrl._CxRunCtrlCount++;
        handlerName = "__slEvent"+count;
        
        window[handlerName]=handler;
    }
    else
    {
        handlerName = null;
    }
    return handlerName;
}