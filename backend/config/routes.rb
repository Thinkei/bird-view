Rails.application.routes.draw do
  # For details on the DSL available within this file, see https://guides.rubyonrails.org/routing.html
  get '/healthz', to: 'healthz#index'

  # @inject examples
get '/api/v1/events', to: 'api/v1/events#index'
end
