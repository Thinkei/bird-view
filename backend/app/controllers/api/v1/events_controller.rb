module Api
  module V1
    class EventsController < ::ApplicationController
      def index
        service = ::Events::GetEvents.call

        render json: GoogleJsonResponse.render(
          service.data
        ).to_json
      end
    end
  end
end
