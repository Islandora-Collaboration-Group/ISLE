/**
<doc:module date="2001-06-12">
   <doc:module>exslt:date-time</doc:module>
   <doc:name>dates</doc:name>
   <doc:version>2.0</doc:version>
   <doc:language>exslt:javascript</doc:language>
   <doc:meta>
      <doc:author email="chris@bayes.co.uk" url="http://www.bayes.co.uk">Chris Bayes</doc:author>
      <doc:summary>Implementation of EXSLT - Dates (http://www.exslt.org/date)</doc:summary>
      <doc:todo>_validDuration is not 100%</doc:todo>
      <doc:todo>All timezone code.</doc:todo>
   </doc:meta>
</doc:module>
**/
var gsSuppliedFormat = "";
var gsLang = "en";
var gaLang = new Array();
// en 
gaLang["en"] = new Array();
gaLang["en"]["months"] = new Array("January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December");
gaLang["en"]["shortMonths"] = new Array("Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec");
gaLang["en"]["days"] = new Array("Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday");
gaLang["en"]["shortDays"] = new Array("Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat");


var gaDayNames = new Array();
gaDayNames["sunday"] = 0;gaDayNames["monday"] = 1;gaDayNames["tuesday"] = 2;gaDayNames["wednesday"] = 3;gaDayNames["thursday"] = 4;gaDayNames["friday"] = 5;gaDayNames["saturday"] = 6;
var gaMonthCnt = new Array(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
var gaMonthCntL = new Array(31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
function _isLeap(y){return ( ((y%4==0)&(y%100!=0))|(y%400==0) );}
function _dayOfWeek(y,d){return ( ((y-1)+Math.floor((y-1)/4)-Math.floor((y-1)/100)+Math.floor((y-1)/400)+d)%7);}
function _dayInYear(y,m,d){return (m==0?d:eval( (_isLeap(y)?gaMonthCntL.slice(0,m).join("+"):gaMonthCnt.slice(0,m).join("+")) )+(d*1) );}
/**
<doc:function date="2001-06-12">
	<doc:name>add</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="no"></doc:arg>
		<doc:arg name="duration" type="string" default="''" optional="no"></doc:arg>
	</doc:args>
</doc:function>
**/
function add(){//
	var d1,d2;
	if (((d1 = _validDate(arguments[0], "xs:dateTime")) ||
		 (d1 = _validDate(arguments[0], "xs:date")) ||
		 (d1 = _validDate(arguments[0], "xs:gYearMonth")) ||
		 (d1 = _validDate(arguments[0], "xs:gYear"))
		) && 
		(d2 = _validDuration(arguments[1])))
		return _formatDate(d2.addDate(d1), gsSuppliedFormat);
	return '';
}
/**
<doc:function date="2001-06-12">
	<doc:name>addDuration</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="duration" type="string" default="''" optional="no"></doc:arg>
		<doc:arg name="duration" type="string" default="''" optional="no"></doc:arg>
	</doc:args>
</doc:function>
**/
function addDuration(duration1, duration2){
	if ((d1 = _validDuration(duration1)) && (d2 = _validDuration(duration2))){
		var d1mnth = d1.years *12 + d1.months;
		var d1secs = d1.days*86400 + d1.hours*3600 + d1.minutes*60 + (d1.seconds+'.'+d1.fraction)*1;
		var d2mnth = d2.years *12 + d2.months;
		var d2secs = d2.days*86400 + d2.hours*3600 + d2.minutes*60 + (d2.seconds+'.'+d2.fraction)*1;
		var mnthdiff = (d1.pm+d1mnth)*1 + (d2.pm+d2mnth)*1;
		var secsdiff = (d1.pm+d1secs)*1 + (d2.pm+d2secs)*1;
		if ((mnthdiff < 0 && secsdiff > 0) || (mnthdiff > 0 && secsdiff < 0))
			return '';
		var secs = secsdiff % 60;
		var pm = (mnthdiff<0||secsdiff<0?'-':'');
		mnthdiff = Math.abs(mnthdiff);
		secsdiff = Math.abs(secsdiff);
		return _formatDuration(new Duration(pm, 0, mnthdiff, Math.floor(secsdiff/86400), Math.floor((secsdiff%86400)/3600), Math.floor((secsdiff%3600)/60), Math.floor(secs), Math.floor(secs%1)));
	}
	return '';
}
/**
<doc:function date="2001-06-12">
	<doc:name>date</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function date(){//
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
			(oDate = _validDate(arguments[0], "xs:date"))
			)
			return _formatDate(oDate, "xs:date");
		else
			return '';
	}else
		return _formatDate(new Date(), "xs:date");
}
/**
<doc:function date="2001-06-12">
	<doc:name>dateTime</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="string" />
	<doc:args />
</doc:function>
**/
function dateTime(){//must be UTC/Z
	if (arguments.length > 0)
		dateTime_invalidArgument.error;
	var oDate = new Date();
	return _formatDate(oDate, "xs:dateTime");
}
/**
<doc:function date="2001-06-12">
	<doc:name>dayAbbreviation</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="date-format" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="context" type="context" default="null" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function dayAbbreviation(){
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
		    (oDate = _validDate(arguments[0], "xs:date"))
		   )
			return _dayAbbreviation(oDate.getDay(), arguments[1], arguments[2]);
		else
			return '';
	}else
		return _dayAbbreviation(new Date().getDay(), arguments[1], arguments[2]);
}
function _dayAbbreviation(num, dfName, ctx){
	if (ctx != null){
		var nodes = ctx.nextNode.selectNodes(
			"//*[local-name()='date-format' and namespace-uri()='http://exslt.org/dates-and-times' and @name='" + dfName +"' ]" +
			"/*[local-name()='days' and namespace-uri()='http://exslt.org/dates-and-times']" +
			"/*[local-name()='day' and namespace-uri()='http://exslt.org/dates-and-times']/@abbr");
		if (nodes != null)
			return nodes[num].nodeValue;
		else
			return '';
	}else{ //we could try local
		return gaLang[gsLang]["shortDays"][num*1];
	}
}
/**
<doc:function date="2001-06-12">
	<doc:name>dayInMonth</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="number" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function dayInMonth(){//
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
			(oDate = _validDate(arguments[0], "xs:date")) ||
			(oDate = _validDate(arguments[0], "xs:gMonthDay")) ||
			(oDate = _validDate(arguments[0], "xs:gDay"))
		    )
			return oDate.getDate();
		else
			return Number.NaN
	}else
		return new Date().getDate();
}
/**
<doc:function date="2001-06-12">
	<doc:name>dayInWeek</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="number" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="date-format" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="context" type="context" default="null" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function dayInWeek(){//
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
		    (oDate = _validDate(arguments[0], "xs:date"))
		   )
			return _dayInWeek(oDate, arguments[1], arguments[2]);
		else
			return Number.NaN;
	}else
		return _dayInWeek(new Date(), arguments[1], arguments[2]);
}
function _dayInWeek(oDate, dfName, ctx){
	var fdow = 0;
	if (ctx != null){
		var nodes = ctx.nextNode.selectSingleNode(
			"//*[local-name()='date-format' and namespace-uri()='http://exslt.org/dates-and-times' and @name='" + dfName +"' ]" +
			"/@first-day-of-week");
		if (nodes != null)
			fdow = gaDayNames[nodes.nodeValue];
	}
	return (oDate.getDay()-fdow<0?oDate.getDay()-fdow+7:oDate.getDay()-fdow)+1;
}
/**
<doc:function date="2001-06-12">
	<doc:name>dayInYear</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="number" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function dayInYear(){
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
			(oDate = _validDate(arguments[0], "xs:date"))
			)
			return _dayInYear(oDate.getFullYear(), oDate.getMonth(), oDate.getDate());
		else
			return Number.NaN;
	}else{
		var oDate = new Date()
		return _dayInYear(oDate.getFullYear(), oDate.getMonth(), oDate.getDate());
	}
}
/**
<doc:function date="2001-06-12">
	<doc:name>dayName</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="date-format" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="context" type="context" default="null" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function dayName(){
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
		    (oDate = _validDate(arguments[0], "xs:date"))
		   )
			return _dayName(oDate.getDay(), arguments[1], arguments[2]);
		else
			return '';
	}else
		return _dayName(new Date().getDay(), arguments[1], arguments[2]);
}
function _dayName(num, dfName, ctx){
	if (ctx != null){
		var nodes = ctx.nextNode.selectNodes(
			"//*[local-name()='date-format' and namespace-uri()='http://exslt.org/dates-and-times' and @name='" + dfName +"' ]" +
			"/*[local-name()='days' and namespace-uri()='http://exslt.org/dates-and-times']" +
			"/*[local-name()='day' and namespace-uri()='http://exslt.org/dates-and-times']/text()");
		if (nodes != null)
			return nodes[num].nodeValue;
		else
			return '';
	}else
		return gaLang[gsLang]["days"][num*1];
}
/**
<doc:function date="2001-06-12">
	<doc:name>dayOfWeekInMonth</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="number" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="date-format" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="context" type="context" default="null" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function dayOfWeekInMonth(){
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
			(oDate = _validDate(arguments[0], "xs:date"))
			)
			return _dayOfWeekInMonth(oDate, arguments[1], arguments[2]);
		else
			return Number.NaN;
	}else
		return _dayOfWeekInMonth(new Date(), arguments[1], arguments[2]);	
}
function _dayOfWeekInMonth(oDate, dfName, ctx){
	var fdow = 0;
	if (ctx != null){
		var nodes = ctx.nextNode.selectSingleNode(
			"//*[local-name()='date-format' and namespace-uri()='http://exslt.org/dates-and-times' and @name='" + dfName +"' ]" +
			"/@first-day-of-week");
		if (nodes != null)
			fdow = gaDayNames[nodes.nodeValue];
	}
	if (fdow > (oDate.getDay()-1))
		return Math.floor((oDate.getDate())/7)+1;
	else
		return Math.floor((oDate.getDate())/7) + 1;
}
/**
<doc:function date="2001-06-12">
	<doc:name>difference</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function difference(){
	if (((d1 = _validDate(arguments[0], "xs:dateTime")) ||
		 (d1 = _validDate(arguments[0], "xs:date")) ||
		 (d1 = _validDate(arguments[0], "xs:gYearMonth")) ||
		 (d1 = _validDate(arguments[0], "xs:gYear"))
		) &&
		((d2 = _validDate(arguments[1], "xs:dateTime")) ||
		 (d2 = _validDate(arguments[1], "xs:date")) ||
		 (d2 = _validDate(arguments[1], "xs:gYearMonth")) ||
		 (d2 = _validDate(arguments[1], "xs:gYear"))
		 )
		){// truncation
		var diffMil = d2.valueOf() - d1.valueOf();
		return _formatDuration(diffMil);
	}
	return Number.NaN;
}
/**
<doc:function date="2001-06-12">
	<doc:name>duration</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="seconds" type="number" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function duration(){
	var oDate;
	if (arguments.length > 0){
		var nN = new Number(arguments[0]);
		if (isNaN(nN))
			return '';
		else
			return _formatDuration(arguments[0] * 1000);
	}else
		return _formatDuration(seconds() * 1000);
}
/**
<doc:function date="2001-06-12">
	<doc:name>formatDate</doc:name>
	<doc:version>1.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="no"></doc:arg>
		<doc:arg name="format" type="string" default="''" optional="no"></doc:arg>
		<doc:arg name="date-format" type="string" default="''" optional="no"></doc:arg>
		<doc:arg name="context" type="document" default="null" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function formatDate(date, format, dateF, ctx){
	var oDate;
	var re;
	var retString = "";
	if ((oDate = _validDate(date, "xs:dateTime")) ||
		(oDate = _validDate(date, "xs:date")) ||
		(oDate = _validDate(date, "xs:time")) ||
		(oDate = _validDate(date, "xs:gYearMonth")) ||
		(oDate = _validDate(date, "xs:gYear")) ||
		(oDate = _validDate(date, "xs:gMonthDay")) ||
		(oDate = _validDate(date, "xs:gMonth")) ||
		(oDate = _validDate(date, "xs:gDay"))
		){
		for (var i=0; i < format.length;){
			var s = i;
			switch(format.substr(i, 1)){
				case "G"://        era designator          (Text)              AD
					while (format.substr(i, 1)=="G"){i++;}
					if (oDate.getFullYear() < 0){
						retString += "BC";
					}else{
						retString += "AD";
					}
					break;
				case "y"://        year                    (Number)            1996
					while (format.substr(i, 1)=="y"){i++;}
					if (i-s > 2){
						retString += oDate.getFullYear();
					}else{
						retString += oDate.getFullYear().toString().substring(4-(i-s));
					}
					break;
				case "M"://        month in year           (Text &amp; Number)     July &amp; 07
					while (format.substr(i, 1)=="M"){i++;}
					if (i-s <= 2){
						retString += pad(oDate.getMonth()+1);
					}else{
						retString += _monthName(oDate.getMonth(), dateF, ctx);
					}
					break;
				case "d"://        day in month            (Number)            10
					while (format.substr(i, 1)=="d"){i++;}
					retString += pad(oDate.getDate());
					break;
				case "h"://        hour in am/pm (1~12)    (Number)            12
					while (format.substr(i, 1)=="h"){i++;}
					if (oDate.getHours() > 12){
						retString += pad(oDate.getHours()-12);
					}else{
						retString += pad(oDate.getHours());
					}
					break;
				case "H"://        hour in day (0~23)      (Number)            0
					while (format.substr(i, 1)=="H"){i++;}
					retString += oDate.getHours();
					break;
				case "m"://        minute in hour          (Number)            30
					while (format.substr(i, 1)=="m"){i++;}
					retString += pad(oDate.getMinutes());
					break;
				case "s"://        second in minute        (Number)            55
					while (format.substr(i, 1)=="s"){i++;}
					retString += pad(oDate.getSeconds());
					break;
				case "S"://        millisecond             (Number)            978
					while (format.substr(i, 1)=="S"){i++;}
					retString += oDate.getMilliseconds();
					break;
				case "E"://        day in week             (Text)              Tuesday
					while (format.substr(i, 1)=="E"){i++;}
					retString += dayInWeek(oDate, dateF, ctx);
					break;
				case "D"://        day in year             (Number)            189
					while (format.substr(i, 1)=="D"){i++;}
					retString += _dayInYear(oDate.getFullYear(), oDate.getMonths(), oDate.getDate());
					break;
				case "F"://        day of week in month    (Number)            2 (2nd Wed in July)
					while (format.substr(i, 1)=="F"){i++;}
					retString += dayOfWeekInMonth(oDate, dateF, ctx);
					break;
				case "w"://        week in year            (Number)            27
					while (format.substr(i, 1)=="w"){i++;}
					retString += weekInYear(oDate, dateF, ctx);
					break;
				case "W"://        week in month           (Number)            2
					while (format.substr(i, 1)=="W"){i++;}
					retString += weekInMonth(oDate, dateF, ctx);
					break;
				case "a"://        am/pm marker            (Text)              PM
					while (format.substr(i, 1)=="a"){i++;}
					if (oDate.getHours() > 12 ){
						retString += "PM"
					}else{
						retString += "AM"
					}
					break;
				case "k"://        hour in day (1~24)      (Number)            24
					while (format.substr(i, 1)=="k"){i++;}
					retString += oDate.getHours();
					break;
				case "K"://        hour in am/pm (0~11)    (Number)            0
					while (format.substr(i, 1)=="K"){i++;}
					if (oDate.getHours() > 12){
						retString += oDate.getHours()-12;
					}else{
						retString += oDate.getHours();
					}
					break;
				case "z"://        time zone               (Text)              Pacific Standard Time
				
				
				
					while (format.substr(i, 1)=="z"){i++;}
					var tzo = oDate.getTimezoneOffset();
					retString += (tzo < 0?"-":"+") + pad(Math.abs(tzo / 60)) + ":" + pad(tzo % 60);
 					break;
 				case "'"://        escape for text         (Delimiter)
 					if (format.substr(i+1, 1) == "'"){
 						i++;
 						while (format.substr(i, 1)=="'"){i++;}
 						retString += "'";
 					}else{
 						i++;
 						while (format.substr(i, 1)!="'" && i <= format.length){retString += format.substr(i++, 1);}
 						if (i >= format.length)return '';
 						i++;
 					}
 					break;
				default:
					retString += format.substr(i, 1);
					i++;
					break;
			}
		}
	}
	return retString;
}
/**
<doc:function date="2001-06-12">
	<doc:name>hourInDay</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="number" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function hourInDay(){//
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
		    (oDate = _validDate(arguments[0], "xs:time"))
		   )
			return oDate.getHours();
		else
			return Number.NaN;
	}else
		return new Date().getHours();
}
/**
<doc:function date="2001-06-12">
	<doc:name>leapYear</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="boolean" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function leapYear(){
	if (arguments.length > 0 && arguments[0] != ''){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
			(oDate = _validDate(arguments[0], "xs:date")) ||
			(oDate = _validDate(arguments[0], "xs:gYearMonth")) ||
			(oDate = _validDate(arguments[0], "xs:gYear"))
		    )
			return (_isLeap(oDate.getFullYear())?true:false);
		else
			return Number.NaN;
	}else
		return (_isLeap(new Date().getFullYear())?true:false);
}
/**
<doc:function date="2001-06-12">
	<doc:name>minuteInHour</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="number" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function minuteInHour(){//
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
		    (oDate = _validDate(arguments[0], "xs:time"))
		   )
			return oDate.getMinutes();
		else
			return Number.NaN;
	}else
		return new Date().getMinutes();
}
/**
<doc:function date="2001-06-12">
	<doc:name>monthAbbreviation</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="date-format" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="context" type="context" default="null" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function monthAbbreviation(){//
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
			(oDate = _validDate(arguments[0], "xs:date")) ||
			(oDate = _validDate(arguments[0], "xs:gYearMonth")) ||
			(oDate = _validDate(arguments[0], "xs:gMonthDay"))
		    )
			return _monthAbbreviation(oDate.getMonth(), arguments[1], arguments[2]);
		else
			return '';
	}else
		return _monthAbbreviation(new Date().getMonth(), arguments[1], arguments[2]);
}
function _monthAbbreviation(num, dfName, ctx){//
	if (ctx != null){
		var nodes = ctx.nextNode.selectNodes(
			"//*[local-name()='date-format' and namespace-uri()='http://exslt.org/dates-and-times' and @name='" + dfName +"' ]" +
			"/*[local-name()='months' and namespace-uri()='http://exslt.org/dates-and-times']" +
			"/*[local-name()='month' and namespace-uri()='http://exslt.org/dates-and-times']/@abbr");
		if (nodes != null)
			return nodes[num].nodeValue;
		else
			return '';
	}else{ //we could try local
		return gaLang[gsLang]["shortMonths"][num*1];
	}
}
/**
<doc:function date="2001-06-12">
	<doc:name>monthInYear</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="number" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function monthInYear(){//
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
			(oDate = _validDate(arguments[0], "xs:date")) ||
			(oDate = _validDate(arguments[0], "xs:gYearMonth")) ||
			(oDate = _validDate(arguments[0], "xs:gMonthDay")) ||
			(oDate = _validDate(arguments[0], "xs:gYear"))
		    )
			return oDate.getMonth()+1;
		else
			return Number.NaN;
	}else
		return new Date().getMonth()+1;
}
/**
<doc:function date="2001-06-12">
	<doc:name>monthName</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="date-format" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="context" type="context" default="null" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function monthName(){//
	if (arguments.length > 0){// && arguments[0] != ''
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
			(oDate = _validDate(arguments[0], "xs:date")) ||
			(oDate = _validDate(arguments[0], "xs:gYearMonth")) ||
			(oDate = _validDate(arguments[0], "xs:gMonthDay"))
		    )
			return _monthName(oDate.getMonth(), arguments[1], arguments[2]);
		else
			return '';
	}else
		return _monthName(new Date().getMonth(), arguments[1],  arguments[2]);
}
function _monthName(num, dfName, ctx){//
	if (ctx != null){
		var nodes = ctx.nextNode.selectNodes(
			"//*[local-name()='date-format' and namespace-uri()='http://exslt.org/dates-and-times' and @name='" + dfName +"' ]" +
			"/*[local-name()='months' and namespace-uri()='http://exslt.org/dates-and-times']" +
			"/*[local-name()='month' and namespace-uri()='http://exslt.org/dates-and-times']/text()");
		if (nodes != null)
			return nodes[num].nodeValue;
		else
			return '';
	}else
		return gaLang[gsLang]["months"][num*1];
}
/**
<doc:function date="2001-06-12">
	<doc:name>parseDate</doc:name>
	<doc:version>1.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="input-format" type="string" default="''" optional="no"></doc:arg>
		<doc:arg name="date-format" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="context" type="document" default="null" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function parseDate(date, inFormat, dfName, ctx){//0001
	var oDate;
	if (inFormat == ""){
		if ((oDate = _validDate(date, "xs:dateTime")) ||
			(oDate = _validDate(date, "xs:date")) ||
			(oDate = _validDate(date, "xs:time")) ||
			(oDate = _validDate(date, "xs:gYearMonth")) ||
			(oDate = _validDate(date, "xs:gYear")) ||
			(oDate = _validDate(date, "xs:gMonthDay")) ||
			(oDate = _validDate(date, "xs:gMonth")) ||
			(oDate = _validDate(date, "xs:gDay"))
			){
			return _formatDate(oDate, "xs:dateTime");
		}
	}else{
		if (oDate = _validDate(date, inFormat))
         return _formatDate(oDate, dfName);
		else
			return '';
	}
}
/**
<doc:function date="2001-06-12">
	<doc:name>secondInMinute</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="number" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function secondInMinute(){//
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
		    (oDate = _validDate(arguments[0], "xs:time"))
		   )
			return oDate.getSeconds();
		else
			return Number.NaN;
	}else
		return new Date().getSeconds();
}
/**
<doc:function date="2001-06-12">
	<doc:name>seconds</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="number" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function seconds(){//
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
			(oDate = _validDate(arguments[0], "xs:date")) ||
			(oDate = _validDate(arguments[0], "xs:gYearMonth")) ||
			(oDate = _validDate(arguments[0], "xs:gYear"))
			)
			return oDate.valueOf()/1000;
		else 
			if (oDate = _validDuration(arguments[0]))
				return oDate.Seconds();
			else
				return Number.NaN;
	}else
		return Math.floor(new Date().valueOf()/1000);//current local date/time
}
/**
<doc:function date="2001-06-12">
	<doc:name>sum</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="durations" type="node-set" default="''" optional="no"></doc:arg>
	</doc:args>
</doc:function>
**/
function sum(ctx){
	var sDate = "P0S";
	if (typeof(ctx) == "object"){
		if (ctx.length){
			for (var i=0; i < ctx.length; i++){
				ctxN  = ctx.item(i);
				if (ctxN.nodeType == 1){
					if (!(sDate = addDuration(sDate, _wander(ctxN))))
						return '';
				}
				if (ctxN.nodeType == 2){
					if (!(sDate = addDuration(sDate, ctxN.nodeValue)))
						return '';
				}
			}
		}else{
			if (!(sDate = addDuration(sDate, ctx.nodeValue)))
					return '';
		}
	}else{
		return '';
	}
	return sDate;
}
function   _wander(ctx){
	var sDate = "P0S";
	for (var i=0; i < ctx.childNodes.length; i++){
		ctxN = ctx.childNodes[i];
		switch(ctxN.nodeType){
			case 1:
				if (!(sDate = addDuration(sDate, _wander(ctxN))))
						return '';
				break;
			case 3:
				if (!(sDate = addDuration(sDate, ctxN.nodeValue)))
						return '';
				break;
			default:
				break;
		}
	}
	return sDate;
}
/**
<doc:function date="2001-06-12">
	<doc:name>time</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function time(){//
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime"))||
			(oDate = _validDate(arguments[0], "xs:time"))
		)
			return _formatDate(oDate, "xs:time");
		else
			return '';
	}else
		return _formatDate(new Date(), "xs:time");
}
/**
<doc:function date="2001-06-12">
	<doc:name>weekInMonth</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="number" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="date-format" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="context" type="context" default="null" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function weekInMonth(){
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
			(oDate = _validDate(arguments[0], "xs:date"))
			)
			return _weekInMonth(oDate, arguments[1], arguments[2]);
		else
			return Number.NaN;
	}else
		return _weekInMonth(new Date(), arguments[1], arguments[2]);	
}
function _weekInMonth(oDate, dfName, ctx){
	var fdow = 0;
	if (ctx != null){
		var nodes = ctx.nextNode.selectSingleNode(
			"//*[local-name()='date-format' and namespace-uri()='http://exslt.org/dates-and-times' and @name='" + dfName +"' ]" +
			"/@first-day-of-week");
		if (nodes != null)
			fdow = gaDayNames[nodes.nodeValue];
	}
	var f = new Date(oDate);
	f.setDate(1)
	var fd = f.getDay()-1;
	var dt = oDate.getDate() + fd - fdow;
	return Math.floor((dt)/7)+1;
}
/**
<doc:function date="2001-06-12">
	<doc:name>weekInYear</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="number" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="date-format" type="string" default="''" optional="yes"></doc:arg>
		<doc:arg name="context" type="context" default="null" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function weekInYear(){
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
			(oDate = _validDate(arguments[0], "xs:date"))
			)
			return _weekInYear(oDate, arguments[1], arguments[2]);
	}else 
		return _weekInYear(new Date());
	return '';
}
function _weekInYear(oDate, dfName, ctx){
	var fdow = 0;
	if (ctx != null){
		var nodes = ctx.nextNode.selectSingleNode(
			"//*[local-name()='date-format' and namespace-uri()='http://exslt.org/dates-and-times' and @name='" + dfName +"' ]" +
			"/@first-day-of-week");
		if (nodes != null)
			fdow = gaDayNames[nodes.nodeValue];
		return Math.ceil(_dayInYear(oDate.getFullYear(), oDate.getMonth(), oDate.getDate()+(7-fdow)) / 7);
	}else
		return Math.ceil(_dayInYear(oDate.getFullYear(), oDate.getMonth(), oDate.getDate()) / 7);
}
/**
<doc:function date="2001-06-12">
	<doc:name>year</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="number" />
	<doc:args>
		<doc:arg name="date" type="string" default="''" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function year(){//
	if (arguments.length > 0){
		var oDate;
		if ((oDate = _validDate(arguments[0], "xs:dateTime")) ||
			(oDate = _validDate(arguments[0], "xs:date")) ||
			(oDate = _validDate(arguments[0], "xs:gYearMonth")) ||
			(oDate = _validDate(arguments[0], "xs:gYear"))
		    )
			return oDate.getFullYear();
		else
			return Number.NaN;
	}else
		return new Date().getFullYear();
}
/**
Private functions.
**/
function _validDate(date, format){
	var parts;
	var oDate = new Date(-62135596800000); // Mon Jan 1 00:00:00 UTC 1
	switch(format){
		case "xs:dateTime": //(CCYY-MM-DDThh:mm:ss)
			var re = new RegExp("^([0-9]{4})-([0-9]{2})-([0-9]{2})(T([0-9]{2})(:([0-9]{2})(:([0-9]{2}))?)?)?(Z)?([\+\-][0-9]{2}:[0-9]{2})?$");
			if (parts=re.exec(date)){
				if (parts[10] == "Z"){
					oDate.setFullYear(parts[1], parts[2]-1, parts[3]);
					oDate.setHours(parts[5], parts[7], parts[9]);
					oDate.setMinutes(oDate.getMinutes()-oDate.getTimezoneOffset());
					gsSuppliedFormat = "xs:dateTime";
					return oDate;
				}else{
					oDate.setFullYear(parts[1], parts[2]-1, parts[3]);
					oDate.setHours(parts[5], parts[7], parts[9]);
					gsSuppliedFormat = "xs:dateTime";
					return oDate;
				}
			}
			break;
		case "xs:date": //(CCYY-MM-DD) 
			var re = new RegExp("^([0-9]{4})-([0-9]{2})-([0-9]{2})$");
			if (parts=re.exec(date)){
				oDate.setFullYear(parts[1], parts[2]-1, parts[3]);
				gsSuppliedFormat = "xs:date";
				return oDate;
			}
			break;
		case "xs:time": //(hh:mm:ss)
			var re = new RegExp("^([0-9]{2})(:([0-9]{2})(:([0-9]{2}))?)?(Z)?$");
			if (parts=re.exec(date)){
				if (parts[9] == "Z"){
					oDate.setHours(parts[1], parts[3], parts[5]);
					oDate.setMinutes(oDate.getMinutes()-oDate.getTimezoneOffset());
					gsSuppliedFormat = "xs:time";
					return oDate;
				}else{
					oDate.setHours(parts[1], parts[3], parts[5]);
					gsSuppliedFormat = "xs:time";
					return oDate;
				}
			}
			break;
		case "xs:gYearMonth": //(CCYY-MM)
			var re = new RegExp("^([0-9]{4})-([0-9]{2})$");
			if (parts=re.exec(date)){
				oDate.setFullYear(parts[1], parts[2]-1);
				gsSuppliedFormat = "xs:gYearMonth";
				return oDate;
			}
			break;
		case "xs:gYear": //(CCYY)
			var re = new RegExp("^([0-9]{4})$");
			if (parts=re.exec(date)){
				oDate.setFullYear(parts[1]);
				gsSuppliedFormat = "xs:gYear";
				return oDate;
			}
			break;
		case "xs:gMonthDay": //(^--MM-DD)
			var re = new RegExp("--([0-9]{2})-([0-9]{2})");
			if (parts=re.exec(date)){
				oDate.setMonth(parts[1]-1, parts[2]);
				gsSuppliedFormat = "xs:gMonthDay";
				return oDate;
			}
			break;
		case "xs:gMonth": //(--MM--)
			var re = new RegExp("^--([0-9]{2})--$");
			if (parts=re.exec(date)){
				oDate.setMonth(parts[1]-1);
				gsSuppliedFormat = "xs:gMonth";
				return oDate;
			}
			break;
		case "xs:gDay": //(---DD)
			var re = new RegExp("^---([0-9]{2})$");
			if (parts=re.exec(date)){
				oDate.setDate(parts[1]);
				gsSuppliedFormat = "xs:gDay";
				return oDate;
			}
			break;
		default:
			// format string 
			var Z = "";
			for (var i=0,j=0; i < format.length & oDate.toString() != "NaN";){
				var s = j;
				switch(format.substr(i, 1)){
					case "G"://        era designator          (Text)              AD
						while (format.substr(i, 1)=="G"){i++;j++;}
						j++;
						if (date.substr(s,j-s) == "BC" || date.substr(s,j-s) == "bc"){
							oDate.setFullYear(-oDate.getFullYear);
						}
						break;
					case "y"://        year                    (Number)            1996
						while (format.substr(i, 1)=="y"){i++;j++;}
						oDate.setYear(date.substr(s,j-s));
						break;
					case "M"://        month in year           (Text &amp; Number)     July &amp; 07
						while (format.substr(i, 1)=="M"){i++;j++;}
						if (i-s <= 2){
							oDate.setMonth(date.substr(s,j-s)-1);
						}else{
							oDate.setMonth(_monthNumber(date.substr(s,j-s)));
						}
						break;
					case "d"://        day in month            (Number)            10
						while (format.substr(i, 1)=="d"){i++;j++;}
						oDate.setDate(date.substr(s,j-s));
						break;
					case "h"://        hour in am/pm (1~12)    (Number)            12
						while (format.substr(i, 1)=="h"){i++;j++;}
						oDate.setHours(date.substr(s,j-s));
						break;
					case "H"://        hour in day (0~23)      (Number)            0
						while (format.substr(i, 1)=="H"){i++;j++;}
						oDate.setHours(date.substr(s,j-s));
						break;
					case "m"://        minute in hour          (Number)            30
						while (format.substr(i, 1)=="m"){i++;j++;}
						oDate.setMinutes(date.substr(s,j-s));
						break;
					case "s"://        second in minute        (Number)            55
						while (format.substr(i, 1)=="s"){i++;j++;}
						oDate.setSeconds(date.substr(s,j-s));
						break;
					case "S"://        millisecond             (Number)            978
						while (format.substr(i, 1)=="S"){i++;j++;}
						oDate.setMilliseconds(date.substr(s,j-s));
						break;
					case "E"://        day in week             (Text)              Tuesday
						while (format.substr(i, 1)=="E"){i++;j++;}
						while (format.substr(i+1, 1)!=date.substr(j, 1)){j++;}
						break;
					case "D"://        day in year             (Number)            189
						while (format.substr(i, 1)=="D"){i++;j++;}
						while (format.substr(i+1, 1)!=date.substr(j, 1)){j++;}
						break;
					case "F"://        day of week in month    (Number)            2 (2nd Wed in July)
						while (format.substr(i, 1)=="F"){i++;j++;}
						break;
					case "w"://        week in year            (Number)            27
						while (format.substr(i, 1)=="w"){i++;j++;}
						while (format.substr(i+1, 1)!=date.substr(j, 1)){j++;}
						break;
					case "W"://        week in month           (Number)            2
						while (format.substr(i, 1)=="W"){i++;j++;}
						while (format.substr(i+1, 1)!=date.substr(j, 1)){j++;}
						break;
					case "a"://        am/pm marker            (Text)              PM
						while (format.substr(i, 1)=="a"){i++;j++;}
						j++;
						if (date.substr(s,j-s) == "PM" || date.substr(s,j-s) == "pm")
							oDate.setHours(oDate.getHours()+12);
						break;
					case "k"://        hour in day (1~24)      (Number)            24
						while (format.substr(i, 1)=="k"){i++;j++;}
						oDate.setHours(date.substr(s,j-s));
						break;
					case "K"://        hour in am/pm (0~11)    (Number)            0
						while (format.substr(i, 1)=="K"){i++;j++;}
						oDate.setHours(date.substr(s,j-s));
						break;
					case "z"://        time zone               (Text)              Pacific Standard Time
						while (format.substr(i, 1)=="z"){i++;j++;}
						while (format.substr(i+1, 1)!=date.substr(j, 1)){j++;}
						//oDate.setMinutes(oDate.getMinutes() - oDate.getTimezoneOffset());
						Z = date.substr(s,j-s);
 						break;
 					case "'"://        escape for text         (Delimiter)
 						if (format.substr(i+1, 1) == "'"){
 							i+=2;
 						}else{
 							i++;
 							while (format.substr(i, 1)!="'"){i++;j++;}
 							i++;
 						}
 						break;
					default:
						if (format.substr(i, 1) != date.substr(j, 1)){
							return false;
						}
						i++;j++;
						break;
				}
			}
			if (j < date.length)
				return false;
			if (oDate.toString() != "NaN"){
				if (Z != ""){
					var re = new RegExp("([\+\-][0-9]{2})?:([0-9]{2})?");
					if (parts=re.exec(Z)){
						// check GMT/BST
						var tzo = parts[1]*60 + parts[2];
						//if (tzo != new Date().getTimezoneOffset()){
						//	oDate.setMinutes(oDate.getMinutes() - (oDate.getTimezoneOffset() - tzo))
						//}
					}
				}
				gsSuppliedFormat = "custom";
				return oDate;
			}
			break;
	}
	return false;
}
function _formatDate(oDate, format){
	if (oDate == Number.NaN)return Number.NaN;
	switch(format){
		case "xs:dateTime": //(CCYY-MM-DDThh:mm:ss)
			var tzo = oDate.getTimezoneOffset(); 
			return oDate.getFullYear() + "-" + pad(oDate.getMonth()+1) + "-" + pad(oDate.getDate()) + "T" +
				   pad(oDate.getHours()) + ":" + pad(oDate.getMinutes()) + ":" + pad(oDate.getSeconds()) + (tzo < 0?"-":"+") + pad(Math.abs(tzo/60)) + ":" + pad(tzo % 60);
			break;
		case "xs:date": //(CCYY-MM-DD) 
			return oDate.getFullYear() + "-" + pad(oDate.getMonth()+1) + "-" + pad(oDate.getDate());
			break;
		case "xs:time": //(hh:mm:ss)
			var tzo = oDate.getTimezoneOffset(); 
			return pad(oDate.getHours()) + ":" + pad(oDate.getMinutes()) + ":" + pad(oDate.getSeconds()) + (tzo < 0?"-":"+") + pad(Math.abs(tzo/60)) + ":" + pad(tzo % 60);
			break;
		case "xs:gYearMonth": //(CCYY-MM)
			return oDate.getFullYear() + "-" + pad(oDate.getMonth()+1);
			break;
		case "xs:gYear": //(CCYY)
			return oDate.getFullYear();
			break;
		case "xs:gMonthDay": //(--MM-DD)
			return pad(oDate.getMonth() + 1) + "-" + pad(oDate.getDate());
			break;
		case "xs:gMonth": //(--MM--)
			return pad(oDate.getMonth()+1);
			break;
		case "xs:gDay": //(---DD)
			return pad(oDate.getDate());
			break;
	}
	return false;
}
function pad(v){
	return (v<10?"0"+v:v);
}

function _validDuration(d){
        var parts;
        var re = new RegExp("^([\-])?P(([0-9]+)Y)?(([0-9]+)M)?(([0-9]+)D)?((T)?(([0-9]+)H)?(([0-9]+)M)?((([0-9]+)(\.([0-9]+))?)S)?)?$");
        if (parts = re.exec(d)){
                if ((/T$/.exec(d)) || (parts[9] == "T" && parts[11]+parts[13]+parts[16]+parts[18]== ""))// ~(:+(|) duh!!!
                        return Number.NaN;
                return new Duration(parts[1], parts[3], parts[5], parts[7], parts[11], parts[13], parts[16], parts[18]);
        }
        return Number.NaN;
}

function Duration(pm, years, months, days, hours, minutes, seconds, fraction){
	this.pm = pm;
	this.years = years*1;
	this.months = months*1;
	this.days = days*1;
	this.hours = hours*1;
	this.minutes = minutes*1;
	this.seconds = seconds*1;
	this.fraction = fraction*1;
	this.milliseconds = eval('0.'+fraction) * 100;
	this.mask = ((this.years?1:0)<<6) + ((this.months?1:0)<<5) + ((this.days?1:0)<<4) + ((this.hours?1:0)<<3) + ((this.minutes?1:0)<<2) + ((this.seconds?1:0)<<1) + (this.milliseconds?1:0);
	this.addDate = function(d){d.setFullYear(eval(this.pm+this.years)+d.getFullYear(),eval(this.pm+this.months)+d.getMonth(),eval(this.pm+this.days)+d.getDate());d.setHours(eval(this.pm+this.hours)+d.getHours(),eval(this.pm+this.minutes)+d.getMinutes(),eval(this.pm+this.seconds)+d.getSeconds());d.setMilliseconds(eval(this.pm+this.milliseconds));return d;}
	this.Seconds = function(){if (this.years || this.months)return Number.NaN;return this.days*giDayMill + this.hours*giHourMill + this.minutes*giMinMill + this.seconds*giSecMill;}
}
var giYearMill = 31536000000;
var giMonthMill = 2592000000;
var giDayMill = 86400000;
var giHourMill = 3600000;
var giMinMill = 60000;
var giSecMill = 1000;
function _formatDuration(du){
	if (typeof(du) == "object"){
		var pm = du.pm;
		var y = du.years;
		var m = du.months;
		var d = du.days;
		var h = du.hours;
		var n = du.minutes;
		var s = du.seconds + du.milliseconds/1000;
		
		if (y+m+d+h+n+s == 0)
			return 'P0S';
		return pm + "P" + 
				(y!=0?y + "Y":'') + 
				(m!=0?m + "M":'') + 
				(d!=0?d + "D":'') + 
				(h+n+s!=0?"T":'') +
				(h!=0?h + "H":'') + 
				(n!=0?n + "M":'') + 
				(s!=0?s + "S":'');
	}else{
		var pm = (du<0?'-':'');
		du = Math.abs(du);
		var d = Math.floor((du) / giDayMill);
		var h = Math.floor((du % giDayMill) / giHourMill);
		var n = Math.floor((du % giHourMill) / giMinMill);
		var s = Math.floor((du % giMinMill) / giSecMill) + (du % giSecMill)/1000;
		return pm + "P" + 
				(d!=0?d + "D":'') + 
				(h+n+s!=0?"T":'') +
				(h!=0?h + "H":'') + 
				(n!=0?n + "M":'') + 
				(s!=0?s + "S":'');
	}
}
