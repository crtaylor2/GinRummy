#include "GinRummyRestServer.h"

#include <json/json.h>

GinRummyRestServer::GinRummyRestServer(std::string url) : m_listener(url), ginRummy()
{
    m_listener.support(web::http::methods::GET, std::bind(&GinRummyRestServer::handle_get, this, std::placeholders::_1));
}

//////////////////////////////////////////////////////////////////////
/// Handles HTTP GET requests
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummyRestServer::handle_get(web::http::http_request message)
{
    std::string response;

    std::vector<std::string> paths = web::http::uri::split_path(web::http::uri::decode(message.relative_uri().path()));
    if(paths.empty())
    {
        message.reply(web::http::status_codes::NotFound, "Unsupported url of " + message.relative_uri().path() + "\n");
        return;
    }
    else if(paths[0] == "sortbyruns")
    {
        response = ginRummy.SortByRuns();
    }
    else if(paths[0] == "sortbysets")
    {
        response = ginRummy.SortBySets();
    }
    else if(paths[0] == "showcomputerhand")
    {
        response = ginRummy.ShowComputerHand();
    }
    else if(paths[0] == "hidecomputerhand")
    {
        response = ginRummy.HideComputerHand();
    }
    else if(paths[0] == "choosediscard")
    {
        response = ginRummy.ChooseDiscard();
    }
    else if(paths[0] == "choosefacedown")
    {
        response = ginRummy.ChooseFaceDown();
    }
    else if(paths[0] == "pass")
    {
        response = ginRummy.Pass();
    }
    else if(paths[0] == "discard")
    {
        int idx = std::stoi(paths[1]) - 1;
        response = ginRummy.Discard(idx);
    }
    else if(paths[0] == "computersturn")
    {
        response = ginRummy.ComputersTurn();
    }
    else if(paths[0] == "knock")
    {
        response = ginRummy.Knock();
    }
    else if(paths[0] == "gin")
    {
        response = ginRummy.Gin();
    }
    else
    {
        message.reply(web::http::status_codes::NotFound, "Unsupported url of " + message.relative_uri().path() + "\n");
        return;
    }

    message.reply(web::http::status_codes::OK, createJsonResponse(response));
    return;
}

//////////////////////////////////////////////////////////////////////
/// Creates a JSON response for the Gin Rummy state
///
/// Returns: std::string (state, in JSON)
//////////////////////////////////////////////////////////////////////
std::string GinRummyRestServer::createJsonResponse(const std::string& response) const
{
    Json::Value json;
    Json::Value playerHand;
    Json::Value computerHand;

    json["Response"] = response;
    json["PlayerScore"] = ginRummy.getPlayerScore();
    json["ComputerScore"] = ginRummy.getComputerScore();

    for(Card& card : ginRummy.getComputerHand())
    {
        Json::Value c;
        c["suit"] = card.SuitString();
        c["value"] = card.ValueString();
        c["string"] = card.CardToString();
        playerHand.append(c);
    }

    for(Card& card : ginRummy.getPlayerHand())
    {
        Json::Value c;
        c["suit"] = ginRummy.getShowingComputerHand() ? card.SuitString() : "";
        c["value"] = ginRummy.getShowingComputerHand() ? card.ValueString() : "";
        c["string"] = ginRummy.getShowingComputerHand() ? card.CardToString() : "";
        computerHand.append(c);
    }

    Json::Value discard;
    discard["suit"] = ginRummy.getDiscardBack().SuitString();
    discard["value"] = ginRummy.getDiscardBack().ValueString();
    discard["string"] = ginRummy.getDiscardBack().CardToString();
    json["discard"] = discard;

    json["playerturn"] = ginRummy.getPlayerTurn();

    json["PlayerHand"] = playerHand;
    json["ComputerHand"] = computerHand;

    Json::StyledWriter writer;
    return writer.write(json);
}
