#include "Global.h"

QStringList parseQualitiesFromOutput(const QString & output, bool & online)
{
    online = false;
    QStringList qualities;

    foreach(QString line, output.split('\n'))
    {
        if(line.startsWith(OUTPUT_QUALITIES_LINE_BEGIN))
        {
            online = true;
            qualities = line.remove(OUTPUT_QUALITIES_LINE_BEGIN)
                            .remove('\r').remove('\n')
                            .split(QUALITIES_SEPARATOR);
            break;
        }
    }

    return qualities;
}

QString twitchChatFromUrl(const QString & url)
{
    /* For an address twitch twitch.tv/streamerName,
       The corresponding chat is twitch.tv/chat/embed?channel=streamerName&popout_chat=true */
    QString chatUrl;

    QStringList urlComponents = url.split("/");
    if(urlComponents.last().isEmpty())
        urlComponents.removeLast();
    QString streamerName = urlComponents.last();
    urlComponents.removeLast();
    chatUrl = urlComponents.join("/");
    chatUrl += TWITCH_CHAT_URL_EXTENSION.arg(streamerName);
    
    return chatUrl;
}

bool operator==(const Stream & lValue, const Stream & rValue)
{
    return (lValue.url == rValue.url);
}