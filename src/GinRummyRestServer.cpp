#include "GinRummyRestServer.h"

#include <json/json.h>

GinRummyRestServer::GinRummyRestServer(std::string url) : m_listener(url), ginRummy()
{
    m_listener.support(web::http::methods::GET, std::bind(&GinRummyRestServer::handle_get, this, std::placeholders::_1));
    //m_listener.support(web::http::methods::POST, std::bind(&GinRummyRestServer::handle_post, this, std::placeholders::_1));
}

void GinRummyRestServer::handle_get(web::http::http_request message)
{
    std::vector<std::string> paths = web::http::uri::split_path(web::http::uri::decode(message.relative_uri().path()));
    if(paths.empty())
    {
        message.reply(web::http::status_codes::NotFound, "Unsupported url of " + message.relative_uri().path() + "\n");
        return;
    }
    else if(paths[0] == "sortbyruns")
    {
        ginRummy.SortByRuns();
    }
    else if(paths[0] == "sortbysets")
    {
        ginRummy.SortBySets();
    }
    else if(paths[0] == "showcomputerhand")
    {
        ginRummy.ShowComputerHand();
    }
    else if(paths[0] == "hidecomputerhand")
    {
        ginRummy.HideComputerHand();
    }
    else if(paths[0] == "discard")
    {
        ginRummy.ChooseDiscard();
    }
    else if(paths[0] == "pass")
    {
        ginRummy.Pass();
    }
    else
    {
        message.reply(web::http::status_codes::NotFound, "Unsupported url of " + message.relative_uri().path() + "\n");
        return;
    }

    message.reply(web::http::status_codes::OK, createJsonResponse());
    return;
}

/*void GinRummyRestServer::handle_post(web::http::http_request message)
{

}*/

std::string GinRummyRestServer::createJsonResponse() const
{
    Json::Value response;
    Json::Value playerHand;
    Json::Value computerHand;

    response["PlayerScore"] = ginRummy.getPlayerScore();
    response["ComputerScore"] = ginRummy.getComputerScore();

    for(Card& card : ginRummy.getComputerHand())
    {
        Json::Value c;
        c["suit"] = card.SuitString();
        c["value"] = card.ValueString();
        playerHand.append(c);
    }

    for(Card& card : ginRummy.getPlayerHand())
    {
        Json::Value c;
        c["suit"] = ginRummy.getShowingComputerHand() ? card.SuitString() : "";
        c["value"] = ginRummy.getShowingComputerHand() ? card.ValueString() : "";
        computerHand.append(c);
    }

    response["PlayerHand"] = playerHand;
    response["ComputerHand"] = computerHand;

    Json::StyledWriter writer;
    return writer.write(response);
}
