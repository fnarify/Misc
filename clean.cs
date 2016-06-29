using System;
using System.IO;
using System.Collections.Specialized;
using System.Text.RegularExpressions;
using System.Text;

/**
 * Cleans a filtered html file output from Microsoft Word.
 * Also removes nearly all tags bar <table>, and cleans up the table formatting if needed.
 *
 * However, this leaves some Windows control characters (^M) in the file. If you're using
 * Vim you can run the command:
 * :%s/<Ctrl-V><Ctrl-M>/\r/g
 * OR
 * :%s/<Ctrl-Q><Ctrl-M>/\r/g
 * for Windows.
 * You can also just use dos2unix/unix2dox or:
 * set fileformat=unix
 * ...
 *
 * The final command that removes some trailing newlines, also leaves a space before each
 * newline. This is easy to fix with any other Regex parser, in Vim you can run:
 * :%s/ \n/\r/g
 */
namespace MyNameSpace
{
    class MyClass
    {
        static void Main(string[] args)  
        {
            if (args.Length == 0 || String.IsNullOrEmpty(args[0]))  
            {
                Console.WriteLine("No filename provided.");  
                return;  
            }
            string filepath = args[0];  
            if (Path.GetFileName(filepath) == args[0])  
            {
                filepath = Path.Combine(Environment.CurrentDirectory, filepath);  
            }
            if (!File.Exists(args[0]))  
            {
                Console.WriteLine("File doesn't exist.");  
            }

            string html = File.ReadAllText(filepath);  
            Console.WriteLine("input html is " + html.Length + " chars");  
            html = CleanHtml(html);
            
            filepath = Path.GetFileNameWithoutExtension(filepath) + ".modified.htm";  
            File.WriteAllText(filepath, html);  
            Console.WriteLine("cleaned html is " + html.Length + " chars");  
        }

        // Removes all FONT and SPAN tags, and all Class and Style attributes.
        // Designed to get rid of non-standard Microsoft Word HTML tags.
        //
        // Extended to remove other tags that weren't needed.
        private static string CleanHtml(string html)
        { 
            // start by completely removing all unwanted tags 
            html = Regex.Replace(html, @"<[/]?(font|span|xml|del|ins|[ovwxp]:\w+)[^>]*?>", "", RegexOptions.IgnoreCase); 
            // then run another pass over the html (twice), removing unwanted attributes 
            html = Regex.Replace(html, @"<([^>]*)(?:class|lang|style|size|face|[ovwxp]:\w+)=(?:'[^']*'|""[^""]*""|[^\s>]+)([^>]*)>","<$1$2>",
                    RegexOptions.IgnoreCase); 
            html = Regex.Replace(html, @"<([^>]*)(?:class|lang|style|size|face|[ovwxp]:\w+)=(?:'[^']*'|""[^""]*""|[^\s>]+)([^>]*)>","<$1$2>",
                    RegexOptions.IgnoreCase); 
            // Remove image and ref tags.
            html = Regex.Replace(html, @"<[/]?(a|img)[^>]+>", "", RegexOptions.IgnoreCase);
            // Remove width/col/align tags in tables.
            html = Regex.Replace(html, @"(width|colspan|valign|rowspan)=[a-zA-Z0-9]+", "", RegexOptions.IgnoreCase);
            // Remove non-breaking spaces, and unreadable characters that show up.
            html = html.Replace("&nbsp;", "");
            html = html.Replace("�", "");
            // Remove all paragraph tags.
            html = Regex.Replace(html, @"<(p|div)\s*(align=[a-zA-Z]+)*\s*>[\n]?", "", RegexOptions.IgnoreCase);
            // Remove all <br clear> tags.
            html = Regex.Replace(html, @"<br(\s|\n)*clear=all\s*>[\n]?", "", RegexOptions.IgnoreCase);
            // Remove <style> tag.
            html = Regex.Replace(html, @"<style>[\s\S]*</style>", "", RegexOptions.IgnoreCase);
            /* PRIMARILY FOR TABLES */
            html = Regex.Replace(html, @"<[/]?(a|div|p)\s*>", "", RegexOptions.IgnoreCase);
            html = Regex.Replace(html, @"<(td|table)(\s|\n)+", "<$1", RegexOptions.IgnoreCase);
            html = Regex.Replace(html, @"<td>(\s|\n)+", "<td>", RegexOptions.IgnoreCase);
            html = Regex.Replace(html, @"(\s|\n)+</td>", "</td>", RegexOptions.IgnoreCase);
            html = Regex.Replace(html, @"\r\n\s\s([^<])", " $1", RegexOptions.IgnoreCase);
            html = Regex.Replace(html, @"border=[0-9]+ cellspacing=[0-9]+ cellpadding=[0-9]+", " class=\"wikitable\"", RegexOptions.IgnoreCase);
            // Replace header tags with corresponding mediawiki variant.
            html = Regex.Replace(html, @"<[/]?h[12]\s*>", "==", RegexOptions.IgnoreCase);
            html = Regex.Replace(html, @"<[/]?h[34]\s*>", "===", RegexOptions.IgnoreCase);
            // Optionally remove all newlines between tags.
            html = Regex.Replace(html, @"[^table|td|tr|th]\s*>(\r\n|\n|\s)+<\s*/[^table|td|tr|th]", ">\r<", RegexOptions.IgnoreCase);
            // Remove <html>, <head> and <body> tags.
            html = Regex.Replace(html, @"<[/]?(html|body)(.*?)>", "", RegexOptions.IgnoreCase);
            html = Regex.Replace(html, @"<head>[\s\S]*</head>", "", RegexOptions.IgnoreCase);
            // Remove all newlines between words and replace with spaces.
            html = Regex.Replace(html, @"(\w+)?[\r]\n(\w+)", "$1 $2", RegexOptions.IgnoreCase);
            return html;
        }

        /*
        static string CleanWordHtml(string html)  
        {
        StringCollection sc = new StringCollection();  
        // get rid of unnecessary tag spans (comments and title)
        sc.Add(@"<!--(w|W)+?-->");  
        sc.Add(@"<title>(w|W)+?</title>");  
        // Get rid of classes and styles
        sc.Add(@"s?class=w+");  
        sc.Add(@"s+style='[^']+'");  
        // Get rid of unnecessary tags
        sc.Add(  
        @"<(meta|link|/?o:|/?style|/?div|/?std|/?head|/?html|body|/?body|/?span|![)[^>]*?>");
        // Get rid of empty paragraph tags
        sc.Add(@"(<[^>]+>)+&nbsp;(</w+>)+");  
        // remove bizarre v: element attached to <img> tag
        sc.Add(@"s+v:w+=""[^""]+""");  
        // remove extra lines
        sc.Add(@"(nr){2,}");  
        foreach (string s in sc)  
        {
        html = Regex.Replace(html, s, "", RegexOptions.IgnoreCase);  
        }
        return html;  
        }

        static string FixEntities(string html)  
        {
        NameValueCollection nvc = new NameValueCollection();  
        nvc.Add("\"", "&ldquo;");  
        nvc.Add("\"", "&rdquo;");  
        nvc.Add("â€“", "&mdash;");  
        foreach (string key in nvc.Keys)  
        {
        html = html.Replace(key, nvc[key]);  
        }
        return html;  
        }
        */
    }
}
